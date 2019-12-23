# POST_S76G
Power-on Self Test for LilyGO&#174; & SoftRF **T-Motion** (AcSiP S76G based) LoRaWAN GNSS tracker

# Illustrations

* Serial console log (115200, 8N1)

<br>

![](https://github.com/lyusupov/POST_S76G/blob/master/documents/images/t-motion-2.jpg)

<br>

* 0.96" I<sup>2</sup>C OLED display ( _if_ _connected_ )

<br>

![](https://github.com/lyusupov/POST_S76G/blob/master/documents/images/t-motion-3.jpg)

<br>

# Sketch build instructions

You should have [**STM32CubeProg**](https://www.st.com/en/development-tools/stm32cubeprog.html) been pre-installed in order to program your T-Motion.<br>
Press **BOOT** button on your T-Motion board, plug it into a PC/laptop USB slot, then release.<br>

1. Follow [these official instructions](https://github.com/stm32duino/wiki/wiki/Getting-Started)
  to install Arduino IDE and [latest **stable** Arduino STM32 Core](https://github.com/stm32duino/Arduino_Core_STM32/releases/tag/1.8.0) (1.8.0)
2. start **Arduino** application
3. open **POST_S76G** sketch from _File_ -> _Open_ menu
4. Select _Tools_ -> _Board_ ->  _Nucleo_64_<br>
5. Select _Tools_ -> _Optimize_ ->  _Fast_ _(-O1)_
6. Select _Tools_ -> _Board_ _part_ _number_ ->  _Nucleo_ _L073RZ_<br>
7. Select _Tools_ -> _C_ _Runtime_ _library_ ->  _Newlib_ _Nano_ _(default)_
8. Select _Tools_ -> _USB_ _speed_ _(if available)_ ->  _Low/Full_ _Speed_
9. Select _Tools_ -> _USB_ _support_ _(if available)_ ->  _CDC_ _(generic_ _'Serial'_ _supersede_ _U(S)ART)_
10. Select _Tools_ -> _U(S)ART_ _support_ ->  _Enabled_ _(no_ _generic_ _'Serial')_
11. Select _Tools_ -> _Upload_ _method_ ->  _STM32CubeProgrammer_ _(DFU)_
12. Select _Tools_ -> _Port_ ->  ``<your Dongle's DFU device name>``
13. Build and upload the sketch using _Sketch_ -> _Upload_

# Credits

Name|Subject
---|---
[LilyGO company](http://www.lilygo.cn/)|[TTGO T-Motion](https://twitter.com/lilygo9/status/1189494231003324416)
[Frederic Pillon](https://github.com/fpistm)|[Arduino Core for STM32](https://github.com/stm32duino/Arduino_Core_STM32)
[Oliver Kraus](https://github.com/olikraus)|[U8g2 monochrome LCD, OLED and eInk library](https://github.com/olikraus/u8g2)
