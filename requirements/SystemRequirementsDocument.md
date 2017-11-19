# 1.0 MOOS Multiplexer Module {#SRD001 }

This module provides a multi-channel multiplexer block for MOOS.

## 1.1 Software Structure & Dependencies {#SRD002 }

### 1.1.1 SRD003 {#SRD003 }

This module **shall** be implemented as a MOOS Appcasting App as describe in the (http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php)[MOOS-IvP wiki].

### 1.1.2 SRD004 {#SRD004 }

This module **shall** use (http://www.rapidjson.org)[rapidjson] as the JSON parse/unparse library.

## 1.2 MOOS Startup Configuration Options {#SRD005 }

### 1.2.1 SRD006 {#SRD006 }

The configuration file **must** have one or more ```block``` entries. Each ```block``` entry **must** be a JSON object conforming to schema/block_schema.json. If any entries do not conform, this module **shall** log an error and exit.

### 1.2.2 SRD007 {#SRD007 }

Each ```block``` entry **must** have a ```ChannelType``` key, which names a MOOS type.

#### 1.2.2.1 SRD008 {#SRD008 }

The output variable **shall** be of this type.

#### 1.2.2.2 SRD009 {#SRD009 }

The input variables **must** be of this type. If they are not, they **shall** be ignored.

### 1.2.3 SRD010 {#SRD010 }

Each ```block``` entry **must** have a ```True``` key which is a JSON object.

#### 1.2.3.1 SRD011 {#SRD011 }

Each True object **may** have a ```function``` key. If it is missing, its value **shall** default to ```variable```.

##### 1.2.3.1.1 SRD012 {#SRD012 }

If the value of this key is ```fixed```, the module **shall** treat this as a fixed value.

##### 1.2.3.1.2 SRD013 {#SRD013 }

If the value of this key is ```variable```, the module **shall** treat this as a variable value controlled by a named MOOS community variable.

#### 1.2.3.2 SRD014 {#SRD014 }

Each True object **must** have a ```var``` key.

##### 1.2.3.2.1 SRD015 {#SRD015 }

If the value of the ```function``` key is ```variable```, then this key must be a string that names a MOOS community variable of the same type specified in ```ChannelType```. If this condition is not met, the module **shall** log an error and exit.

##### 1.2.3.2.2 SRD016 {#SRD016 }

If the value of the ```function``` key is ```fixed```, this key **must** have the same type as named in ```ChannelType```. If this condition is not met, the module **shall** log an error and exit.

### 1.2.4 SRD017 {#SRD017 }

Each ```block``` entry **must** have a ```False``` key which is a JSON object.

#### 1.2.4.1 SRD018 {#SRD018 }

Each False object **may** have a ```function``` key. If it is missing, its value **shall** default to ```variable```.

##### 1.2.4.1.1 SRD019 {#SRD019 }

If the value of this key is ```fixed```, the module **shall** treat this as a fixed value.

##### 1.2.4.1.2 SRD020 {#SRD020 }

If the value of this key is ```variable```, the module **shall** treat this as a variable value controlled by a named MOOS community variable.

#### 1.2.4.2 SRD021 {#SRD021 }

Each False object **must** have a ```var``` key.

##### 1.2.4.2.1 SRD022 {#SRD022 }

If the value of the ```function``` key is ```variable```, then this key must be a string that names a MOOS community variable of the same type specified in ```ChannelType```. If this condition is not met, the module **shall** log an error and exit.

##### 1.2.4.2.2 SRD023 {#SRD023 }

If the value of the ```function``` key is ```fixed```, this key **must** have the same type as named in ```ChannelType```. If this condition is not met, the module **shall** log an error and exit.

### 1.2.5 SRD024 {#SRD024 }

Each ```block``` entry **must** have an ```Output``` key which is a string. This module **shall** publish the output of this block to the variable named by this key.

### 1.2.6 SRD025 {#SRD025 }

Each ```block``` entry **must** have a ```Switch``` key which is a JSON object.

#### 1.2.6.1 SRD026 {#SRD026 }

Each Switch object **must** have a ```var``` key which is a string. The module **shall** subscribe to this variable.

#### 1.2.6.2 SRD027 {#SRD027 }

Each Switch object **may** have a ```type``` key which is a string. This value **must** be either ```BINARY``` or ```DOUBLE```. If it is missing, the value **shall** default to ```BINARY```.

#### 1.2.6.3 SRD028 {#SRD028 }

The variable named by ```var``` must have the type named in this key. If this condition is not met, the module **shall** log an error and exit.

#### 1.2.6.4 SRD029 {#SRD029 }

If the ```type``` key is equal to ```DOUBLE```, then the ```block``` entry **may** have a ```threshold``` key. If it is not present, its value **shall** default to 0.

#### 1.2.6.5 SRD030 {#SRD030 }

If the ```threshold``` key is otherwise present, the module **shall** log an error and exit.

# 2.0 Data Processing {#SRD031 }

## 2.1 SRD032 {#SRD032 }

On each comms tick, this module **shall** read the True, False, and Switch inputs for each block.

## 2.2 SRD033 {#SRD033 }

The following operations **shall** be performed for each block:

### 2.2.1 SRD034 {#SRD034 }

If the Switch input is a BINARY and it is ```true```, then this module **shall** publish the value of the True input to the Output variable.

### 2.2.2 SRD035 {#SRD035 }

If the Switch input is a BINARY and it is ```false```, then this module **shall** publish the value of the False input to the Output variable.

### 2.2.3 SRD036 {#SRD036 }

If the Switch input is a DOUBLE and its value is greater than the specified ```threshold```, then this module **shall** publish the value of the True input to the Output variable.

### 2.2.4 SRD037 {#SRD037 }

If the Switch input is a DOUBLE and its value is less than the specified ```threshold```, then this module **shall** publish the value of the False input to the Output variable.

