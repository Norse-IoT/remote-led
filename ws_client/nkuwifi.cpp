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

#include "nkuwifi.h"

const char* test_root_ca =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIFODCCBCCgAwIBAgISBEFzJpw1t0uAwO31hrZECElhMA0GCSqGSIb3DQEBCwUA\n"
  "MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n"
  "EwJSMzAeFw0yMzAyMDUxMDMwMjNaFw0yMzA1MDYxMDMwMjJaMBsxGTAXBgNVBAMT\n"
  "EGluZm9ybWF0aWNzLnBsdXMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
  "AQDUQrxqY4tqa9owTKUGFERPXO07jjACnywwcyheILwFvdkF7wH/lzbC40WnZOIb\n"
  "/6wR1IKdhMMrIYL5gQYSfCiCk7KVVCkAcBnaRMua3wQnuHPFjkP5fXUUeG84WsR1\n"
  "pJ2rjEIMvEpuMW4SGh9QBPpQHC7/OxCMrqtd/PvuY+euGPbThQa7JWM7HHZ1kHs0\n"
  "4hP3H0b1G+gLbEBQYqUuiQdwTe6Nqm27weyCTx5UqnPuoc3uOSwns+ZTf6jjh7u6\n"
  "6tBPpwGTR9/glIl4IClb1sA1xP3HeYHUTs8ZeNfMx6T1+kXLLC3YkaqSWFzzrTEJ\n"
  "mo/3wQClCDeZmZKhTM0r+9LBAgMBAAGjggJdMIICWTAOBgNVHQ8BAf8EBAMCBaAw\n"
  "HQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYD\n"
  "VR0OBBYEFEVmz2OoSxEt+qtMJ6ibyQsJU/WiMB8GA1UdIwQYMBaAFBQusxe3WFbL\n"
  "rlAJQOYfr52LFMLGMFUGCCsGAQUFBwEBBEkwRzAhBggrBgEFBQcwAYYVaHR0cDov\n"
  "L3IzLm8ubGVuY3Iub3JnMCIGCCsGAQUFBzAChhZodHRwOi8vcjMuaS5sZW5jci5v\n"
  "cmcvMC8GA1UdEQQoMCaCEiouaW5mb3JtYXRpY3MucGx1c4IQaW5mb3JtYXRpY3Mu\n"
  "cGx1czBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsG\n"
  "AQUFBwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQIGCisGAQQB1nkC\n"
  "BAIEgfMEgfAA7gB1AHoyjFTYty22IOo44FIe6YQWcDIThU070ivBOlejUutSAAAB\n"
  "hiFXb40AAAQDAEYwRAIgMU3BcoBmyxNNPda2JsbxbyPDMCLJMx21TRhDECcP05cC\n"
  "IHaLL797CZ7tCG9s9VQx+sqLNRs8rfOfVrXfypY946QFAHUArfe++nz/EMiLnT2c\n"
  "Hj4YarRnKV3PsQwkyoWGNOvcgooAAAGGIVdvoAAABAMARjBEAiB90Bhl29TEIrUM\n"
  "wldGsXuVHp8UhSzzpKWlpRKOnhvfLgIgCwlh/Bg2vfFj2fJHBvJPF+9yb8eeSubE\n"
  "5wKOlkU1XTIwDQYJKoZIhvcNAQELBQADggEBALIawqjQr1BhPhDfGFZSWMXJ3kEf\n"
  "AlFnFxel5CNsH3x2TV1mPpssUQFDNjk3bvUDtYukHYAbw0/2qXVh3aYoQSQe6YUK\n"
  "waVRqJkmSKcBgHU7+baQ9zLDCCu9vR3W4l9vVeHrRwuGw7sqrrdzr+f7Vbh7le3b\n"
  "sRThMmkNfSnr94oD/p1pIrF9MF06TEuj2fCoE8RK1bgLKX2Ruu1KtOERP6mvuBJ5\n"
  "flS89wTbaKPXfn+dcXOKkXaQ6cwU4IGMmndd9TwCDsXQSsK89jafACZh6HJFbJ4K\n"
  "N5qQ3D7Izj1zdDRVLn3yYJZnBqaYr50pOlSvb2OSPQIwPUyJVzq+UTyImUQ=\n"
  "-----END CERTIFICATE-----\n"
  "-----BEGIN CERTIFICATE-----\n"
  "MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n"
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n"
  "WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n"
  "RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n"
  "AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n"
  "R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n"
  "sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n"
  "NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n"
  "Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n"
  "/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n"
  "AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n"
  "Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n"
  "FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n"
  "AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n"
  "Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n"
  "gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n"
  "PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n"
  "ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n"
  "CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n"
  "lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n"
  "avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n"
  "yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n"
  "yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n"
  "hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n"
  "HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n"
  "MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n"
  "nLRbwHOoq7hHwg==\n"
  "-----END CERTIFICATE-----\n"
  "-----BEGIN CERTIFICATE-----\n"
  "MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n"
  "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
  "DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n"
  "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
  "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n"
  "AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n"
  "ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n"
  "wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n"
  "LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n"
  "4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n"
  "bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n"
  "sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n"
  "Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n"
  "FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n"
  "SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n"
  "PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n"
  "TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n"
  "SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n"
  "c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n"
  "+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n"
  "ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n"
  "b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n"
  "U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n"
  "MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n"
  "5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n"
  "9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n"
  "WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n"
  "he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n"
  "Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n"
  "-----END CERTIFICATE-----\n";

WiFiClientSecure client;

WiFiClientSecure* connectToWifi(const char* ssid) {
  int counter = 0;
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);                                                                                 //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA);                                                                                   //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t*)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY));  //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));                      //provide username
  esp_wifi_sta_wpa2_ent_set_password((uint8_t*)EAP_PASSWORD, strlen(EAP_PASSWORD));                      //provide password
  esp_wifi_sta_wpa2_ent_enable();
  WiFi.begin(ssid);  //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) {  //after 30 seconds timeout - reset board (on unsucessful connection)
      ESP.restart();
    }
  }
  client.setCACert(test_root_ca);
  //client.setCertificate(test_client_cert); // for client verification - certificate
  //client.setPrivateKey(test_client_key);  // for client verification - private key
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP());  //print LAN IP
  return &client;
}