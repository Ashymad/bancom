## bancom
Multi-band compressor plugin in JUCE.
![image](https://user-images.githubusercontent.com/15618637/54143396-0aadfa80-442a-11e9-85fa-9bdc22f36d61.png)

Features:
 - Variable number of bands (via Add/Remove buttons followed by Apply)
 - Controls (left-to-right): filter frequency, threshold, ratio, attack, release, make-up gain
 - Intuitive meters: yellow -> level, red -> compresion, green -> make-up, blue -> threshold
 
You can grab the compiled version for Linux from the [releases](https://github.com/Ashymad/bancom/releases) page in standalone, vst and vst3 versions.

## Compile

You will have to change the "Header Search Paths" box to include path to an older version of the VST2 SDK if you want to compile VST2. Otherwise just remove the path from the box and deselect VST2 option from "Plugin Formats"
