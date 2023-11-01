#ifndef NKU_WIFI_H_
#define NKU_WIFI_H_
/*|-----------------------------------------------------------|*/
/*| Enterprise Wifi plus SSL connection to server             |*/
/*| by Bc. Martin Chlebovec, Technical University of Košice   |*/
/*|                                                           |*/
/*| Modded by Chris Brewer with cert chain for negotiating    |*/
/*| SSL connection to iot.informatics.plus                    |*/
/*|                                                           |*/
/*| On success this script will print out headers and content |*/
/*| of a file at https://iot.informatics.plus/wifi.txt        |*/
/*|-----------------------------------------------------------|*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_wpa2.h"
#include <Wire.h>

#include "credentials.h"
/* // includes:
 * #define EAP_ANONYMOUS_IDENTITY "anonymous@example.com" //anonymous identity, leave as anonymous@example.com
 * #define EAP_IDENTITY "YOUR UERSNAME" //user name (email WITHOUT @nku.edu)
 * #define EAP_PASSWORD "Password" // user password
 */

WiFiClientSecure* connectToWifi(const char*);

#endif // NKU_WIFI_H_