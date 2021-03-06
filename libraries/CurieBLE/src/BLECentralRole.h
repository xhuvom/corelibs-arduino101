/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _BLE_CENTRALROLE_H_INCLUDED
#define _BLE_CENTRALROLE_H_INCLUDED
#include "BLECommon.h"
#include "BLEPeripheralHelper.h"
#include "BLECentralHelper.h"
#include "BLERoleBase.h"

class BLECentralRole: public BLERoleBase {
public:
    /**
     * @brief   Start scan
     *
     * @param   none
     *
     * @return  bool    Indicate the success or error
     *
     * @note  none
     */
    bool startScan();
    
    /**
     * @brief   Start scan with scan parameter
     *
     * @param   none
     *
     * @return  bool    Indicate the success or error
     *
     * @note  none
     */
    bool startScan(const bt_le_scan_param_t &scan_param);
    
    /**
     * @brief   Stop scan
     *
     * @param   none
     *
     * @return  bool    Indicate the success or error
     *
     * @note  none
     */
    bool stopScan();
    
    /**
     * @brief   Schedule a connect request to peripheral to establish a connection
     *
     * @param[in]   addr    The MAC address of peripheral device that want to establish connection
     *
     * @param[in]   param   The connetion parameters
     *
     * @return  bool    Indicate the success or error
     *
     * @note  none
     */
    bool connect(const bt_addr_le_t *addr, const bt_le_conn_param_t *param);
    
    /**
     * @brief   Set the scan parameter
     *
     * @param[in]   scan_param      The scan parameter want to be set
     *
     * @return  none
     *
     * @note  none
     */
    void setScanParam(const bt_le_scan_param_t &scan_param);
    
    /**
     * @brief   Get the scan parameter
     *
     * @param   none
     *
     * @return  const bt_le_scan_param_t* The scan parameter that current used
     *
     * @note  none
     */
    const bt_le_scan_param_t* getScanParam();
    
    /**
     * @brief   Discover the peripheral device profile
     *
     * @param   peripheral  The Peripheral that need to discover the profile
     *
     * @return  none
     *
     * @note  none
     */
    void discover(BLEPeripheralHelper &peripheral);
    
    /**
     * @brief   Add an attribute to the BLE Central Device
     *
     * @param[in]   attribute   Attribute to add to Central
     *
     * @return  BleStatus indicating success or error
     *
     * @note  The attribute will used for discover the peripheral handler
     */
    BleStatus addAttribute(BLEAttribute& attribute);
    
    /**
     * @brief Provide a function to be called when events related to this Device are raised
     *
     * @param[in] event    Event type for callback
     * @param[in] callback Pointer to callback function to invoke when an event occurs.
     */
    void setEventHandler(BLERoleEvent event, BLERoleEventHandler callback);
    
    /**
     * @brief   Provide a function to be called when scanned the advertisement
     *
     * @param[in]   advcb   Pointer to callback function to invoke when advertisement received
     *
     * @return  none
     *
     * @note  none
     */
    void setAdvertiseHandler(ble_advertise_handle_cb_t advcb);
    
    /**
     * @brief   Get BLE peripheral helper by conntion
     *
     * @param[in]   conn    The connection object
     *
     * @return  BLEPeripheralHelper*    The BLE peripheral helper
     *
     * @note  none
     */
    BLEPeripheralHelper* peripheral(bt_conn_t *conn);
    
    /**
     * @brief   Get BLE central helper that for APP use
     *
     * @param   none
     *
     * @return  const BLECentralHelper *    The BLE central helper
     *
     * @note  none
     */
    const BLECentralHelper *central(void) const;
    
    /**
     * Setup attributes and start advertising
     *
     * @return bool indicating success or error
     */
    bool begin();
    
    /**
     * @brief   Disconnect the central connected if there is one connected
     *
     * @param   none
     *
     * @return  bool    Indicating success or error
     *
     * @note  none
     */
    bool disconnect();
    
    /**
     * @brief   Get BLE Central instance.
     *
     * @param   none
     *
     * @return  BLECentralRole*  The BLE Central instance
     *
     * @note    Singleton. Only have one object to communicate with 
     *           stack and manage the device
     */
    static BLECentralRole *instance();
    
protected:
    friend void ble_central_device_found(const bt_addr_le_t *addr, 
                                          int8_t rssi, 
                                          uint8_t type,
                                          const uint8_t *ad, 
                                          uint8_t len);
    
    /**
     * @brief   Handle the connected event
     *
     * @param[in]   conn    The object that established the connection
     *
     * @param[in]   err     The code of the process
     *
     * @return  none
     *
     * @note  none
     */
    void handleConnectEvent(bt_conn_t *conn, uint8_t err);
    
    /**
     * @brief   Handle the disconnected event
     *
     * @param[in]   conn    The object that lost the connection
     *
     * @param[in]   reason  The link lost reason
     *
     * @return  none
     *
     * @note  none
     */
    void handleDisconnectEvent(bt_conn_t *conn, uint8_t reason);
    
    /**
     * @brief   Handle the conntion update request
     *
     * @param[in]   conn    The connection object that need to process the update request
     *
     * @param[in]   interval    The connection interval (N*1.25)ms
     *
     * @param[in]   latency     The connection latency
     *
     * @param[in]   timeout     The connection timeout (N*10)ms
     *
     * @return  none
     *
     * @note  none
     */
    void handleParamUpdated(bt_conn_t *conn, 
                            uint16_t interval,
                            uint16_t latency, 
                            uint16_t timeout);
    /**
     * @brief   Handle the advertisement
     *
     * @param[in]   addr    The device's MAC address that send out ADV
     *
     * @param[in]   rssi    The antenna's RSSI
     *
     * @param[in]   type    The advertise type
     *
     * @param[in]   ad      The advertisement RAW data
     *
     * @param[in]   len     The RAW data's length
     *
     * @return  none
     *
     * @note  none
     */
    void handleDeviceFound(const bt_addr_le_t *addr, 
                           int8_t rssi, 
                           uint8_t type,
                           const uint8_t *ad, 
                           uint8_t len);
private:
    BLECentralRole();
    ~BLECentralRole();
    BLEPeripheralHelper* _peripherial[BLE_MAX_CONN_CFG];
    BLECentralHelper    _central;
    bt_le_scan_param_t _scan_param;
    
    static BLECentralRole* _ble_central_ins;
    ble_advertise_handle_cb_t _adv_event_handle;
};

#endif

