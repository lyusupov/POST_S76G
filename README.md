# POST_S76G
Power-on Self Test for TTGO T-Motion (AcSiP S76G based) LoRaWAN GNSS tracker

# Sketch build instructions

You will need to get and connect ST-LINK/V2 USB adapter in order to put the firmware into your T-Motion flash memory.<br>

1. Follow [these official instructions](https://github.com/stm32duino/wiki/wiki/Getting-Started)
  to install Arduino IDE and [latest **stable** Arduino STM32 Core](https://github.com/stm32duino/Arduino_Core_STM32/releases/tag/1.7.0) (1.7.0)
2. start **Arduino** application
3. open **POST_S76G** sketch from _File_ -> _Open_ menu
4. Select _Tools_ -> _Board_ ->  _Nucleo_64_<br>
5. Select _Tools_ -> _Optimize_ ->  _Smallest_ _(-Os_ _default)_
6. Select _Tools_ -> _Board_ _part_ _number_ ->  _Nucleo_ _L073RZ_<br>
7. Select _Tools_ -> _C_ _Runtime_ _library_ ->  _Newlib_ _Nano_ _(default)_
8. Select _Tools_ -> _USB_ _speed_ _(if available)_ ->  _Low/Full_ _Speed_
9. Select _Tools_ -> _USB_ _support_ _(if available)_ ->  _CDC_ _(no_ _generic_ _'Serial')_
10. Select _Tools_ -> _U(S)ART_ _support_ ->  _Enabled_ _(generic_ _'Serial')_
11. Select _Tools_ -> _Upload_ _method_ ->  _STM32CubeProgrammer_ _(SWD)_
12. Select _Tools_ -> _Port_ ->  ``<your ST-LINK/V2 port device name>``
13. Build and upload the sketch using _Sketch_ -> _Upload_
