/*!
 * \file      LoRa.c
 *
 * \brief     LoRaStateMachine based on LoRaMac classA device implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Simon Hoinkis
 */

#include "LoRa.h"
#include "LoRaMac.h"
#include "NvmCtxMgmt.h"
#include "board.h"
#include "utilities.h"

#include <stdio.h>

LoRaMacHandlerAppData_t AppData = {.MsgType = LORAMAC_HANDLER_UNCONFIRMED_MSG,
                                   .Buffer = NULL,
                                   .BufferSize = 0,
                                   .Port = 0};

/*!
 * MAC status strings
 */
const char* MacStatusStrings[] = {
    "OK",                            // LORAMAC_STATUS_OK
    "Busy",                          // LORAMAC_STATUS_BUSY
    "Service unknown",               // LORAMAC_STATUS_SERVICE_UNKNOWN
    "Parameter invalid",             // LORAMAC_STATUS_PARAMETER_INVALID
    "Frequency invalid",             // LORAMAC_STATUS_FREQUENCY_INVALID
    "Datarate invalid",              // LORAMAC_STATUS_DATARATE_INVALID
    "Frequency or datarate invalid", // LORAMAC_STATUS_FREQ_AND_DR_INVALID
    "No network joined",             // LORAMAC_STATUS_NO_NETWORK_JOINED
    "Length error",                  // LORAMAC_STATUS_LENGTH_ERROR
    "Region not supported",          // LORAMAC_STATUS_REGION_NOT_SUPPORTED
    "Skipped APP data",              // LORAMAC_STATUS_SKIPPED_APP_DATA
    "Duty-cycle restricted",         // LORAMAC_STATUS_DUTYCYCLE_RESTRICTED
    "No channel found",              // LORAMAC_STATUS_NO_CHANNEL_FOUND
    "No free channel found",         // LORAMAC_STATUS_NO_FREE_CHANNEL_FOUND
    "Busy beacon reserved time",     // LORAMAC_STATUS_BUSY_BEACON_RESERVED_TIME
    "Busy ping-slot window time", // LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME
    "Busy uplink collision",      // LORAMAC_STATUS_BUSY_UPLINK_COLLISION
    "Crypto error",               // LORAMAC_STATUS_CRYPTO_ERROR
    "FCnt handler error",         // LORAMAC_STATUS_FCNT_HANDLER_ERROR
    "MAC command error",          // LORAMAC_STATUS_MAC_COMMAD_ERROR
    "ClassB error",               // LORAMAC_STATUS_CLASS_B_ERROR
    "Confirm queue error",        // LORAMAC_STATUS_CONFIRM_QUEUE_ERROR
    "Multicast group undefined",  // LORAMAC_STATUS_MC_GROUP_UNDEFINED
    "Unknown error",              // LORAMAC_STATUS_ERROR
};

/*!
 * MAC event info status strings.
 */
const char* EventInfoStatusStrings[] = {
    "OK",                       // LORAMAC_EVENT_INFO_STATUS_OK
    "Error",                    // LORAMAC_EVENT_INFO_STATUS_ERROR
    "Tx timeout",               // LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT
    "Rx 1 timeout",             // LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT
    "Rx 2 timeout",             // LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT
    "Rx1 error",                // LORAMAC_EVENT_INFO_STATUS_RX1_ERROR
    "Rx2 error",                // LORAMAC_EVENT_INFO_STATUS_RX2_ERROR
    "Join failed",              // LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL
    "Downlink repeated",        // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED
    "Tx DR payload size error", // LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR
    "Downlink too many frames loss", // LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS
    "Address fail",                  // LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL
    "MIC fail",                      // LORAMAC_EVENT_INFO_STATUS_MIC_FAIL
    "Multicast fail",                // LORAMAC_EVENT_INFO_STATUS_MULTICAST_FAIL
    "Beacon locked",                 // LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED
    "Beacon lost",                   // LORAMAC_EVENT_INFO_STATUS_BEACON_LOST
    "Beacon not found" // LORAMAC_EVENT_INFO_STATUS_BEACON_NOT_FOUND
};

/*!
 * Application port
 */
static uint8_t AppPort = LORAWAN_APP_PORT;

/*!
 * User application data
 */
uint8_t AppDataBuffer[LORAWAN_APP_DATA_MAX_SIZE];

/*!
 * Indicates if the node is sending confirmed or unconfirmed messages
 */
static uint8_t IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;

/*!
 * Indicates if a new packet can be sent
 */
static bool NextTx = true;

/*!
 * Indicates if LoRaMacProcess call is pending.
 *
 * \warning If variable is equal to 0 then the MCU can be set in low power mode
 */
static uint8_t IsMacProcessPending = 0;

/*!
 * Defines the application data transmission duty cycle
 */
static uint32_t TxDutyCycleTime;

/*!
 * Enum representing the current state of the main state machine
 */
eDeviceState DeviceState = DEVICE_STATE_RESTORE;

void PrintHexBuffer(uint8_t* buffer, uint8_t size)
{
    uint8_t newline = 0;

    for (uint8_t i = 0; i < size; i++)
    {
        if (newline != 0)
        {
            printf("\r\n");
            newline = 0;
        }

        printf("%02X ", buffer[i]);

        if (((i + 1) % 16) == 0)
        {
            newline = 1;
        }
    }
    printf("\r\n");
}

void JoinNetwork(void)
{
    LoRaMacStatus_t status;
    MlmeReq_t mlmeReq;
    mlmeReq.Type = MLME_JOIN;
    mlmeReq.Req.Join.Datarate = LORAWAN_DEFAULT_DATARATE;

    // Starts the join procedure
    status = LoRaMacMlmeRequest(&mlmeReq);
    printf("\r\n###### ===== MLME-Request - MLME_JOIN ==== ######\r\n");
    printf("STATUS      : %s\r\n", MacStatusStrings[status]);

    if (status == LORAMAC_STATUS_OK)
    {
        printf("###### ===== JOINING ==== ######\r\n");
        DeviceState = DEVICE_STATE_SLEEP;
    }
    else
    {
        DeviceState = DEVICE_STATE_CYCLE_JOIN;
    }
}

bool SendFrame(void)
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;

    if (LoRaMacQueryTxPossible(AppDataSize, &txInfo) != LORAMAC_STATUS_OK)
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
        if (IsTxConfirmed == false)
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = AppPort;
            mcpsReq.Req.Unconfirmed.fBuffer = AppDataBuffer;
            mcpsReq.Req.Unconfirmed.fBufferSize = AppDataSize;
            mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = AppPort;
            mcpsReq.Req.Confirmed.fBuffer = AppDataBuffer;
            mcpsReq.Req.Confirmed.fBufferSize = AppDataSize;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
    }

    // Update global variable
    AppData.MsgType = (mcpsReq.Type == MCPS_CONFIRMED)
                          ? LORAMAC_HANDLER_CONFIRMED_MSG
                          : LORAMAC_HANDLER_UNCONFIRMED_MSG;
    AppData.Port = mcpsReq.Req.Unconfirmed.fPort;
    AppData.Buffer = mcpsReq.Req.Unconfirmed.fBuffer;
    AppData.BufferSize = mcpsReq.Req.Unconfirmed.fBufferSize;

    LoRaMacStatus_t status;
    status = LoRaMacMcpsRequest(&mcpsReq);
    printf("\r\n###### ===== MCPS-Request ==== ######\r\n");
    printf("STATUS      : %s\r\n", MacStatusStrings[status]);

    if (status == LORAMAC_STATUS_OK)
    {
        return false;
    }
    return true;
}

void OnTxNextPacketTimerEvent(void* context)
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    TimerStop(&TxNextPacketTimer);

    mibReq.Type = MIB_NETWORK_ACTIVATION;
    status = LoRaMacMibGetRequestConfirm(&mibReq);

    if (status == LORAMAC_STATUS_OK)
    {
        if (mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE)
        {
            // Network not joined yet. Try to join again
            JoinNetwork();
        }
        else
        {
            DeviceState = DEVICE_STATE_SEND;
            NextTx = true;
        }
    }
}

void McpsConfirm(McpsConfirm_t* mcpsConfirm)
{
    printf("\r\n###### ===== MCPS-Confirm ==== ######\r\n");
    printf("STATUS      : %s\r\n", EventInfoStatusStrings[mcpsConfirm->Status]);
    if (mcpsConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK)
    {
    }
    else
    {
        switch (mcpsConfirm->McpsRequest)
        {
        case MCPS_UNCONFIRMED:
        {
            // Check Datarate
            // Check TxPower
            break;
        }
        case MCPS_CONFIRMED:
        {
            // Check Datarate
            // Check TxPower
            // Check AckReceived
            // Check NbTrials
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        default:
            break;
        }
    }
    MibRequestConfirm_t mibGet;
    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm(&mibReq);

    printf("\r\n###### ===== UPLINK FRAME %lu ==== ######\r\n",
           mcpsConfirm->UpLinkCounter);
    printf("\r\n");

    printf("CLASS       : %c\r\n", "ABC"[mibReq.Param.Class]);
    printf("\r\n");
    printf("TX PORT     : %d\r\n", AppData.Port);

    if (AppData.BufferSize != 0)
    {
        printf("TX DATA     : ");
        if (AppData.MsgType == LORAMAC_HANDLER_CONFIRMED_MSG)
        {
            printf("CONFIRMED - %s\r\n",
                   (mcpsConfirm->AckReceived != 0) ? "ACK" : "NACK");
        }
        else
        {
            printf("UNCONFIRMED\r\n");
        }
        PrintHexBuffer(AppData.Buffer, AppData.BufferSize);
    }

    printf("\r\n");
    printf("DATA RATE   : DR_%d\r\n", mcpsConfirm->Datarate);

    mibGet.Type = MIB_CHANNELS;
    if (LoRaMacMibGetRequestConfirm(&mibGet) == LORAMAC_STATUS_OK)
    {
        printf("U/L FREQ    : %lu\r\n",
               mibGet.Param.ChannelList[mcpsConfirm->Channel].Frequency);
    }

    printf("TX POWER    : %d\r\n", mcpsConfirm->TxPower);

    mibGet.Type = MIB_CHANNELS_MASK;
    if (LoRaMacMibGetRequestConfirm(&mibGet) == LORAMAC_STATUS_OK)
    {
        printf("CHANNEL MASK: ");
#if defined(REGION_AS923) || defined(REGION_CN779) || defined(REGION_EU868)    \
    || defined(REGION_IN865) || defined(REGION_KR920) || defined(REGION_EU433) \
    || defined(REGION_RU864)

        for (uint8_t i = 0; i < 1; i++)

#elif defined(REGION_AU915) || defined(REGION_US915) || defined(REGION_CN470)

        for (uint8_t i = 0; i < 5; i++)
#else

#error "Please define a region in the compiler options."

#endif
        {
            printf("%04X ", mibGet.Param.ChannelsMask[i]);
        }
        printf("\r\n");
    }

    printf("\r\n");
}

void McpsIndication(McpsIndication_t* mcpsIndication)
{
    printf("\r\n###### ===== MCPS-Indication ==== ######\r\n");
    printf("STATUS      : %s\r\n",
           EventInfoStatusStrings[mcpsIndication->Status]);
    if (mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK)
    {
        return;
    }

    switch (mcpsIndication->McpsIndication)
    {
    case MCPS_UNCONFIRMED:
    {
        break;
    }
    case MCPS_CONFIRMED:
    {
        break;
    }
    case MCPS_PROPRIETARY:
    {
        break;
    }
    case MCPS_MULTICAST:
    {
        break;
    }
    default:
        break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    if (mcpsIndication->FramePending == true)
    {
        // The server signals that it has pending data to be sent.
        // We schedule an uplink as soon as possible to flush the server.
        OnTxNextPacketTimerEvent(NULL);
    }
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot

    if (mcpsIndication->RxData == true)
    {
        switch (mcpsIndication->Port)
        {
        /// @todo what does port mean in this context?
        case 1: // The application LED can be controlled on port 1 or 2
        case 2:
            break;
        default:
            break;
        }
    }

    const char* slotStrings[] = {
        "1", "2", "C", "C Multicast", "B Ping-Slot", "B Multicast Ping-Slot"};

    printf("\r\n###### ===== DOWNLINK FRAME %lu ==== ######\r\n",
           mcpsIndication->DownLinkCounter);

    printf("RX WINDOW   : %s\r\n", slotStrings[mcpsIndication->RxSlot]);

    printf("RX PORT     : %d\r\n", mcpsIndication->Port);

    if (mcpsIndication->BufferSize != 0)
    {
        printf("RX DATA     : \r\n");
        PrintHexBuffer(mcpsIndication->Buffer, mcpsIndication->BufferSize);
        ParseRxFrame(mcpsIndication->Buffer, mcpsIndication->BufferSize);
    }

    printf("\r\n");
    printf("DATA RATE   : DR_%d\r\n", mcpsIndication->RxDatarate);
    printf("RX RSSI     : %d\r\n", mcpsIndication->Rssi);
    printf("RX SNR      : %d\r\n", mcpsIndication->Snr);

    printf("\r\n");
}

void MlmeConfirm(MlmeConfirm_t* mlmeConfirm)
{
    printf("\r\n###### ===== MLME-Confirm ==== ######\r\n");
    printf("STATUS      : %s\r\n", EventInfoStatusStrings[mlmeConfirm->Status]);
    if (mlmeConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK)
    {
    }
    switch (mlmeConfirm->MlmeRequest)
    {
    case MLME_JOIN:
    {
        if (mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK)
        {
            MibRequestConfirm_t mibGet;
            printf("###### ===== JOINED ==== ######\r\n");
            printf("\r\nOTAA\r\n\r\n");

            mibGet.Type = MIB_DEV_ADDR;
            LoRaMacMibGetRequestConfirm(&mibGet);
            printf("DevAddr     : %08lX\r\n", mibGet.Param.DevAddr);

            printf("\n\r\n");
            mibGet.Type = MIB_CHANNELS_DATARATE;
            LoRaMacMibGetRequestConfirm(&mibGet);
            printf("DATA RATE   : DR_%d\r\n", mibGet.Param.ChannelsDatarate);
            printf("\r\n");
            // Status is OK, node has joined the network
            DeviceState = DEVICE_STATE_SEND;
        }
        else
        {
            // Join was not successful. Try to join again
            JoinNetwork();
        }
        break;
    }
    case MLME_LINK_CHECK:
    {
        if (mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK)
        {
            // Check DemodMargin
            // Check NbGateways
        }
        break;
    }
    default:
        break;
    }
}

void MlmeIndication(MlmeIndication_t* mlmeIndication)
{
    if (mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED)
    {
        printf("\r\n###### ===== MLME-Indication ==== ######\r\n");
        printf("STATUS      : %s\r\n",
               EventInfoStatusStrings[mlmeIndication->Status]);
    }
    if (mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK)
    {
    }
    switch (mlmeIndication->MlmeIndication)
    {
    case MLME_SCHEDULE_UPLINK:
    { // The MAC signals that we shall provide an uplink as soon as possible
        OnTxNextPacketTimerEvent(NULL);
        break;
    }
    default:
        break;
    }
}

void OnMacProcessNotify(void)
{
    IsMacProcessPending = 1;
}

void RunLoRaStateMachine(uint8_t devEui[],
                         uint8_t joinEui[],
                         uint8_t NwkKey[],
                         uint32_t dutyCycle)
{
    LoRaMacPrimitives_t macPrimitives;
    LoRaMacCallback_t macCallbacks;
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    macPrimitives.MacMcpsConfirm = McpsConfirm;
    macPrimitives.MacMcpsIndication = McpsIndication;
    macPrimitives.MacMlmeConfirm = MlmeConfirm;
    macPrimitives.MacMlmeIndication = MlmeIndication;
    macCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    macCallbacks.GetTemperatureLevel = NULL;
    macCallbacks.NvmContextChange = NvmCtxMgmtEvent;
    macCallbacks.MacProcessNotify = OnMacProcessNotify;

    status =
        LoRaMacInitialization(&macPrimitives, &macCallbacks, ACTIVE_REGION);
    if (status != LORAMAC_STATUS_OK)
    {
        printf("LoRaMac wasn't properly initialized, error: %s",
               MacStatusStrings[status]);
        // Fatal error, endless loop.
        while (1)
        {
        }
    }

    while (1)
    {
        // Process Radio IRQ
        if (Radio.IrqProcess != NULL)
        {
            Radio.IrqProcess();
        }
        // Processes the LoRaMac events
        LoRaMacProcess();

        switch (DeviceState)
        {
        case DEVICE_STATE_RESTORE:
        {
            // Try to restore from NVM and query the mac if possible.
            if (NvmCtxMgmtRestore() == NVMCTXMGMT_STATUS_SUCCESS)
            {
                printf("\r\n###### ===== CTXS RESTORED ==== ######\r\n\r\n");
            }
            else
            {
                mibReq.Type = MIB_NWK_KEY;
                mibReq.Param.NwkKey = NwkKey;
                LoRaMacMibSetRequestConfirm(&mibReq);

                // Initialize LoRaMac device unique ID if not already defined in
                // Commissioning.h
                if ((devEui[0] == 0) && (devEui[1] == 0) && (devEui[2] == 0)
                    && (devEui[3] == 0) && (devEui[4] == 0) && (devEui[5] == 0)
                    && (devEui[6] == 0) && (devEui[7] == 0))
                {
                    BoardGetUniqueId(devEui);
                }

                mibReq.Type = MIB_DEV_EUI;
                mibReq.Param.DevEui = devEui;
                LoRaMacMibSetRequestConfirm(&mibReq);

                mibReq.Type = MIB_JOIN_EUI;
                mibReq.Param.JoinEui = joinEui;
                LoRaMacMibSetRequestConfirm(&mibReq);
            }
            DeviceState = DEVICE_STATE_START;
            break;
        }

        case DEVICE_STATE_START:
        {
            TimerInit(&TxNextPacketTimer, OnTxNextPacketTimerEvent);

            mibReq.Type = MIB_PUBLIC_NETWORK;
            mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
            LoRaMacMibSetRequestConfirm(&mibReq);

            mibReq.Type = MIB_ADR;
            mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
            LoRaMacMibSetRequestConfirm(&mibReq);

            mibReq.Type = MIB_RX2_CHANNEL;
            mibReq.Param.Rx2Channel = (RxChannelParams_t){869525000, DR_0};
            LoRaMacMibSetRequestConfirm(&mibReq);

            mibReq.Type = MIB_SYSTEM_MAX_RX_ERROR;
            mibReq.Param.SystemMaxRxError = 20;
            LoRaMacMibSetRequestConfirm(&mibReq);

            LoRaMacStart();

            mibReq.Type = MIB_NETWORK_ACTIVATION;
            status = LoRaMacMibGetRequestConfirm(&mibReq);

            if (status == LORAMAC_STATUS_OK)
            {
                if (mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE)
                {
                    DeviceState = DEVICE_STATE_JOIN;
                }
                else
                {
                    DeviceState = DEVICE_STATE_SEND;
                    NextTx = true;
                }
            }
            break;
        }
        case DEVICE_STATE_JOIN:
        {
            mibReq.Type = MIB_DEV_EUI;
            LoRaMacMibGetRequestConfirm(&mibReq);
            printf("DevEui      : %02X", mibReq.Param.DevEui[0]);
            for (int i = 1; i < 8; i++)
            {
                printf("-%02X", mibReq.Param.DevEui[i]);
            }
            printf("\r\n");
            mibReq.Type = MIB_JOIN_EUI;
            LoRaMacMibGetRequestConfirm(&mibReq);
            printf("AppEui      : %02X", mibReq.Param.JoinEui[0]);
            for (int i = 1; i < 8; i++)
            {
                printf("-%02X", mibReq.Param.JoinEui[i]);
            }
            printf("\r\n");
            printf("AppKey      : %02X", NwkKey[0]);
            for (int i = 1; i < 16; i++)
            {
                printf(" %02X", NwkKey[i]);
            }
            printf("\n\r\n");

            JoinNetwork();

            break;
        }
        case DEVICE_STATE_SEND:
        {
            if (NextTx == true)
            {
                PrepareTxFrame(AppPort);

                NextTx = SendFrame();
            }
            DeviceState = DEVICE_STATE_CYCLE_RUN;
            break;
        }
        case DEVICE_STATE_CYCLE_JOIN:
        {
            DeviceState = DEVICE_STATE_SLEEP;

            TxDutyCycleTime =
                APP_TX_DUTYCYCLE_JOIN_MS
                + randr(-APP_TX_DUTYCYCLE_RND_MS, APP_TX_DUTYCYCLE_RND_MS);

            // Schedule next packet transmission
            TimerSetValue(&TxNextPacketTimer, TxDutyCycleTime);
            TimerStart(&TxNextPacketTimer);
            break;
        }
        case DEVICE_STATE_CYCLE_RUN:
        {
            DeviceState = DEVICE_STATE_SLEEP;

            TxDutyCycleTime =
                dutyCycle
                + randr(-APP_TX_DUTYCYCLE_RND_MS, APP_TX_DUTYCYCLE_RND_MS);

            // Schedule next packet transmission
            TimerSetValue(&TxNextPacketTimer, TxDutyCycleTime);
            TimerStart(&TxNextPacketTimer);
            break;
        }

        case DEVICE_STATE_SLEEP:
        {
            if (NvmCtxMgmtStore() == NVMCTXMGMT_STATUS_SUCCESS)
            {
                printf("\r\n###### ===== CTXS STORED ==== ######\r\n");
            }

            CRITICAL_SECTION_BEGIN();
            if (IsMacProcessPending == 1)
            {
                // Clear flag and prevent MCU to go into low power modes.
                IsMacProcessPending = 0;
            }
            else
            {
                // The MCU wakes up through events
                /// @todo Leads to reset handler call for some reason
                // BoardLowPowerHandler( );
            }
            CRITICAL_SECTION_END();
            break;
        }
        default:
        {
            DeviceState = DEVICE_STATE_START;
            break;
        }
        }
    }
}
