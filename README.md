### `secret.h`

Before flashing application provide a wifi ssid and password into a new file `include/secret.h`. An example of how it should be set up is as follows

```c
#pragma once
#ifndef _SECRET_H
#define _SECRET_H

const char* WIFI_SSID = "<SSID>";
const char* WIFI_PASS = "<PASS>";

#endif // _SECRET_H
```
