### `secrets`

Before flashing application provide a wifi ssid and password into a new file `include/secret.h`. An example of how it should be set up is as follows

`secret.h`
```c
#pragma once
#ifndef _SECRET_H_
#define _SECRET_H_

#include "esp_wifi.h"

wifi_sta_config_t GetSecretStaCfg();

#endif //_SECRET_H_
```

`secret.c`
```c
#include "secret.h"

wifi_sta_config_t GetSecretStaCfg()
{
    wifi_sta_config_t newCfg = {
        .ssid = "<SSID>",
        .password = "<PASS>",
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        .pmf_cfg = {
            .capable = true,
            .required = false
        }
    };

    return newCfg;
}
```
