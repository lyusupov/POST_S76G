# POST_S76G
Power-on Self Test for _(AcSiP S76G based)_ LilyGO&#174; & SoftRF **T-Motion** LoRaWAN GNSS tracker and **T-Impulse** wristband

* [Illustrations](https://github.com/lyusupov/POST_S76G/blob/master/README.md#illustrations)
* [Sketch build instructions](https://github.com/lyusupov/POST_S76G/blob/master/README.md#sketch-build-instructions)
* [Credits](https://github.com/lyusupov/POST_S76G/blob/master/README.md#credits)
* [More information](https://github.com/lyusupov/POST_S76G/blob/master/README.md#more-information)

# Illustrations

* Serial console log (115200, 8N1)
<br>

## T-Motion

![](https://github.com/lyusupov/POST_S76G/blob/master/documents/images/t-motion-2.jpg)

<br>

## T-Impulse

![](https://user-images.githubusercontent.com/5849637/133896429-09c8abae-dddd-4c10-a8a1-46b5fa3df652.png)

<br>

* 0.96" I<sup>2</sup>C OLED display ( _if_ _connected_ )

<br>

![](https://github.com/lyusupov/POST_S76G/blob/master/documents/images/t-motion-3.jpg)

<br>

# Sketch build instructions

You should have [**STM32CubeProg**](https://www.st.com/en/development-tools/stm32cubeprog.html) been pre-installed in order to program your T-Motion board or T-Impulse wristband.<br>
Press **BOOT** button on your T-Motion _(or T-Impulse)_ device, plug it into a PC/laptop USB slot, then release.<br>

1. Follow [these official instructions](https://github.com/stm32duino/wiki/wiki/Getting-Started)
  to install Arduino IDE and [<!-- latest --> **stable** Arduino STM32 Core](https://github.com/stm32duino/Arduino_Core_STM32/releases/tag/1.9.0) (1.9.0)
2. start **Arduino** application
3. open **POST_S76G** sketch from _File_ -> _Open_ menu
4. Select _Tools_ -> _Board_ ->  _Nucleo_64_<br>
5. Select _Tools_ -> _Optimize_ ->  _Smallest_ _(-Os default)_
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
[LilyGO company](http://www.lilygo.cn/)|[TTGO T-Motion](http://www.lilygo.cn/prod_view.aspx?Id=1254) and [T-Impulse](https://s.click.aliexpress.com/e/_AVK1N4)
[Frederic Pillon](https://github.com/fpistm)|[Arduino Core for STM32](https://github.com/stm32duino/Arduino_Core_STM32)
[Oliver Kraus](https://github.com/olikraus)|[U8g2 monochrome LCD, OLED and eInk library](https://github.com/olikraus/u8g2)

# More information

1. [List of AcSiP S7xx resources](https://github.com/lyusupov/POST_S76G/wiki/AcSiP-S7xG-resources)

