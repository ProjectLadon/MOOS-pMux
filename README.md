# MOOS-pMux

A mux block for MOOS. Each channel subscribes to two variables and outputs a third variable chosen from the first two based on the value of a fourth variable. The control variable may either be a BINARY or a DOUBLE. If it is a DOUBLE, values above a given level are treated as true and those below as false. 

## Dependencies
* MOOS-IvP
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse

## Configuration Options
* block -- defines a mux channel with the following JSON schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"id": "MUX_block",
	"type": "object",
	"properties":{
		"ChannelType":{"type":"string","enum":["STRING","DOUBLE","BINARY"]},
		"True":{
			"type":"object",
			"properties": {
				"function":{"type":"string","enum":["fixed","variable"]}
				"var":{
					oneOf":[
						{"type":"string"},
						{"type":"number"},
						{"type":"boolean"}
					]
				}
			},
			"required":["var"],
		},
		"False":{
			"type":"object",
			"properties": {
				"function":{"type":"string","enum":["fixed","variable"]}
				"var":{
					oneOf":[
						{"type":"string"},
						{"type":"number"},
						{"type":"boolean"}
					]
				}
			},
			"required":["var"],
		},
		"Output":{"type":"string"},
		"Switch":{
			"type":"object",
			"properties": {
				"type":{"type":"string","enum":["BINARY","DOUBLE"]}
				"var":{"type":"string"},
				"threshold":{"type":"number"}
			},
			"required":["var"],
		}
	}
}
```