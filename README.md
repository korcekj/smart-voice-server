# ESP8266 API - HTTP Responses

---

- HTTP_OK

```
{
	"status": 200,
	"description": {
		"rate": "success",
		"message": "Operation was successfully done."
	}
}
```

- HTTP_CREATED_OK

```
{
	"status": 201,
	"description": {
		"rate": "error",
		"message": "Operation was successfully done."
	}
}
```

- HTTP_BAD_REQUEST

```
{
	"status": 400,
	"description": {
		"rate": "error",
		"message": "Operation was not successfully done."
	}
}
```

- HTTP_NOT_AUTHORIZED

```
{
	"status": 401,
	"description": {
		"rate": "error",
		"message": "Operation was not authorized."
	}
}
```

- HTTP_NOT_FOUND

```
{
	"status": 404,
	"description": {
		"rate": "error",
		"message": "Operation was not found."
	}
}
```

- HTTP_INTERNAL_SERVER_ERROR

```
{
	"status": 500,
	"description": {
		"rate": "error",
		"message": "Operation was not successfully done."
	}
}
```

# ESP8266 - Hardware models

## ESP8266 - LED Controls

---

### LED Properties

- **LED pin** (uint8_t)
  - number of pin in ESP8266
  - possible from 0 _MIN_ESP8266_PIN_ to 16 _MAX_ESP8266_PIN_
- **LED name** (String)
  _ name of strip
  _ empty _String_ could be defined
- **LED numLeds** (uint16_t)
  - number of lads on strip
  - possible from 1 _MIN_NUM_OF_LEDS_
- **LED status** (uint8_t)
  - status of strip
  - 0 -> _L_OFF_ 1 -> _L_ON_
- **LED brightness** (uint8_t)
  - possible from 0 _MIN_BRIGHTNESS_ to 100 _MAX_BRIGHTNESS_
- **LED wait** (int)
  - delay time between transition to another pixel trinity
  - possible from 0 _MIN_MS_ to 255 _MAX_MS_
- **LED mode** (uint8_t)
  - 0 -> _MODE_ZERO_
  - 1 -> _MODE_ONE_
  - 2 -> _MODE_TWO_
- **LED colors** (std::vector)
  - **LED color** (std::map)
    - **r** (uint8_t)
    - **g** (uint8_t)
    - **b** (uint8_t)
    - **a** (uint8_t)
    - possible from 0 to 255, if value is overflow, reduction will be applied

### LED Modes

0. **All pixels on led strip are on**
   - only one color is defined
   - brightness could be configured
1. **N-pixels are on and they are moving from the beginning to the end**
   - n-pixels -> possible from 1 to 3 include
   - colors(n) -> as was mentioned above
   - brightness could be configured as well
   - wait time could be handy to make beautiful light show
2. **Rainbow show**
   - brightness
   - wait

## ESP8266 - REMOTE Controls

---

### REMOTE Properties

- **REMOTE pin** (uint8_t)
  - number of pin in ESP8266
  - possible from 0 _MIN\*ESP8266_PIN_ to 16 _MAX_ESP8266_PIN_
- **REMOTE name** (String)
  - name of remote controller
  - empty _String_ could be defined
- **REMOTE type** (uint8_t)
  - type of remote controller
  - possible from list of controllers _(0 = SONY)_
- **REMOTE frequency** (uint8_t)
  - frequency of remote controller
  - possible from 0 - 255 kHz
- **REMOTE command** (uint8_t)
  - number to recognise type of command
  - possible from 0 - 255

### REMOTE commands

0. **TURN ON/OFF**
   - send IR signal on device based on TV type
1. **VOLUME UP**
   - send IR signal on device based on TV type
   - step 1
2. **VOLUME DOWN**
   - send IR signal on device based on TV type
   - step 1
3. **CHANNEL UP**
   - send IR signal on device based on TV type
   - step 1
4. **CHANNEL DOWN**
   - send IR signal on device based on TV type
   - step 1
