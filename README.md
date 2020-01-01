# qoroshiro-inverse
GUI for xoroshiro-inverse (https://github.com/pattirudon/xoroshiro-inverse) written in Qt

# Prerequisites

Windows users must have Java installed, and pattirudon's [xoroshiroinverse.jar](https://github.com/pattirudon/xoroshiro-inverse/blob/master/xoroshiroinverse.jar?raw=true) (ver. 1.3) in the same folder as the executable. Linux users must have Qt libraries and Java installed and xoroshiroinverse.jar (ver. 1.3) in the same folder as the executable.

# Usage

Fill the text fields with the required values and click "Generate". To restart the search or do another one, click "Stop". To exit the program, click "Exit". This program acts like a "smart" overlay and even calculates some values by itself. The output of xoroshiroinverse.jar appears at the textbox below.

# Sanity checks and limitations

This program has some basic sanity checks implemented (like not allowing to use incorrect characteristics or some IV combinations). It comes with its own set of limitations (like an ability to input only 3- and 4-IV spreads). This can be fixed, if needed, and in the future there might be a version that allows the user to disable sanity checks completely

# What's next

Tons of stuff. Rewriting some poorly-written code, fixing bugs mostly, and keeping up with the xoroshiro-inverse.

# License

qoroshiro-inverse is released as open source software under the [GPL v3](https://opensource.org/licenses/gpl-3.0.html) license, see the [LICENSE](./LICENSE) file in the project root for the full license text.
