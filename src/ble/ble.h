/**
 * @file
 */

#ifndef BLE_H
#define BLE_H

void ble_task(void *params);

void ble_sendCmdMode(void);
void ble_sendVersion(void);
void ble_sendFactoryReset(void);
void ble_sendReboot(void);
void ble_sendResetServices(void);
void ble_sendAdvertising(void);
void ble_sendFakePacket(void);

#endif