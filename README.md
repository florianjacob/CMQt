# CMQt - interact with the Connection Machine with Qt5 #
In case you ever visited the second floor of the „Infobau“ (building 50.34) of the KIT in Karlsruhe, you probably found
the big black cubes that make up the [Connection Machine][]. Since 2014, it has built-in, RaspberryPi-powered and
Bluetooth-controlled LED matrix in the front panels, to be seen and used by everyone that visits it.

The Connection Machine (CM for short) shows random patterns when idle to mimic it's behaviour when it was still at work,
but with the right smartphone apps, you can now play games on the matrix.

## Connection Machine Specification ##
Using this example code together with the [Connection Machine Specification][], you can write your own apps using Qt
Bluetooth. While the actual example application GUI is written for [SailfishOS][], the C++ code can be used on any Qt5
platform that supports the [QtBluetooth API][].

## Licence ##
GPLv3. See [COPYING](COPYING).

## Other CM Apps ##
Other apps are published in the [CM Arcade][], but they're written in Java and only work on Android.


[CM Arcade]: http://www.teco.kit.edu/cm
[Connection Machine Specification]: http://www.teco.kit.edu/cm/dev/
[SailfishOS]: https://sailfishos.org/
[QtBluetooth API]: http://doc.qt.io/qt-5/qtbluetooth-index.html
[Connection Machine]: http://en.wikipedia.org/wiki/Connection_Machine
