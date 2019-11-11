# ESP8266 - HTTP Responses
- - - -

* HTTP_OK

```
{
	"status": 200,
	"description": {
		"rate": "success",
		"message": "Operation was successfully done."  
	}
}
```

* HTTP_BAD_REQUEST

```
{
	"status": 400,
	"description": {
		"rate": "error",
		"message": "Operation was not successfully done."  
	}
}
```

* HTTP_NOT_FOUND

```
{
	"status": 404,
	"description": {
		"rate": "error",
		"message": "Operation was not found."  
	}
}
```
