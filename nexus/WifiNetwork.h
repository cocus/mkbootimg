/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _WIFI_NETWORK_H
#define _WIFI_NETWORK_H

#include <sys/types.h>

#include "../../../frameworks/base/include/utils/List.h"

class KeyManagementMask {
public:
    static const uint32_t NONE      = 0;
    static const uint32_t WPA_PSK   = 0x01;
    static const uint32_t WPA_EAP   = 0x02;
    static const uint32_t IEEE8021X = 0x04;
    static const uint32_t ALL       = WPA_PSK | WPA_EAP | IEEE8021X;
};

class SecurityProtocolMask {
public:
    static const uint32_t WPA = 0x01;
    static const uint32_t RSN = 0x02;
};

class AuthenticationAlgorithmMask {
public:
    static const uint32_t OPEN   = 0x01;
    static const uint32_t SHARED = 0x02;
    static const uint32_t LEAP   = 0x04;
};

class PairwiseCipherMask {
public:
    static const uint32_t NONE = 0x00;
    static const uint32_t TKIP = 0x01;
    static const uint32_t CCMP = 0x02;
};

class GroupCipherMask {
public:
    static const uint32_t WEP40  = 0x01;
    static const uint32_t WEP104 = 0x02;
    static const uint32_t TKIP   = 0x04;
    static const uint32_t CCMP   = 0x08;
};

class Supplicant;

class WifiNetwork {
    Supplicant *mSuppl;

    /*
     * Unique network id - normally provided by supplicant
     */
    int mNetid;

    /*
     * The networks' SSID. Can either be an ASCII string,
     * which must be enclosed in double quotation marks
     * (ie: "MyNetwork"), or a string of hex digits which
     * are not enclosed in quotes (ie: 01ab7893)
     */
    char *mSsid;

    /*
     * When set, this entry should only be used
     * when associating with the AP having the specified
     * BSSID. The value is a string in the format of an
     * Ethernet MAC address
     */
    char *mBssid;

    /*
     *  Pre-shared key for use with WPA-PSK
     */
    char *mPsk;

    /*
     * Up to four WEP keys. Either in ASCII string enclosed in
     * double quotes, or a string of hex digits
     */
    char *mWepKeys[4];

    /*
     * Default WEP key index, ranging from 0 -> NUM_WEP_KEYS -1
     */
    int mDefaultKeyIndex;

    /*
     * Priority determines the preference given to a network by 
     * supplicant when choosing an access point with which
     * to associate
     */
    int mPriority;

    /*
     * This is a network that does not broadcast it's SSID, so an
     * SSID-specific probe request must be used for scans.
     */
    char *mHiddenSsid;

    /*
     * The set of key management protocols supported by this configuration.
     */
    uint32_t mAllowedKeyManagement;

    /*
     * The set of security protocols supported by this configuration.
     */
    uint32_t mAllowedProtocols;

    /*
     * The set of authentication protocols supported by this configuration.
     */
    uint32_t mAllowedAuthAlgorithms;

    /*
     * The set of pairwise ciphers for WPA supported by this configuration.
     */
    uint32_t mAllowedPairwiseCiphers;

    /*
     * The set of group ciphers for WPA supported by this configuration.
     */
    uint32_t mAllowedGroupCiphers;

public:
    WifiNetwork(Supplicant *suppl);
    virtual ~WifiNetwork();

    int getNetworkId() { return mNetid; }
    const char *getSsid() { return mSsid; }
    const char *getBssid() { return mBssid; }
    const char *getPsk() { return mPsk; }
    const char *getWepKey(int idx) { return mWepKeys[idx]; }
    int getDefaultKeyIndex() { return mDefaultKeyIndex; }
    int getPriority() { return mPriority; }
    const char *getHiddenSsid() { return mHiddenSsid; }
    uint32_t getAllowedKeyManagement() { return mAllowedKeyManagement; }
    uint32_t getAllowedProtocols() { return mAllowedProtocols; }
    uint32_t getAllowedAuthAlgorithms() { return mAllowedAuthAlgorithms; }
    uint32_t getAllowedPairwiseCiphers() { return mAllowedPairwiseCiphers; }
    uint32_t getAllowedGroupCiphers() { return mAllowedGroupCiphers; }

    int setSsid(char *ssid);
    int setBssid(char *bssid);
    int setPsk(char *psk);
    int setWepKey(int idx, char *key);
    int setDefaultKeyIndex(int idx);
    int setPriority(int pri);
    int setHiddenSsid(char *ssid);
    int setAllowedKeyManagement(uint32_t mask);
    int setAllowedProtocols(uint32_t mask);
    int setAllowedPairwiseCiphers(uint32_t mask);
    int setAllowedGroupCiphers(uint32_t mask);
};

typedef android::List<WifiNetwork *> WifiNetworkCollection;

#endif
