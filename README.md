# MEGAHAKUS
A Tesla overlay for 初音ミク Project DIVA MEGA39's.

![screenshot](https://github.com/nastys/MEGAHAKUS/raw/master/preview.jpg)

Licence: GNU GENERAL PUBLIC LICENSE Version 2

Project DIVA Modding 2nd Discord server: https://discord.gg/cvBVGDZ

## Requirements
- [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere)
- 初音ミク Project DIVA MEGA39's 1.0.3
- (recommended) An extracted copy of the game's romfs partition (use [Lockpick_RCM](https://github.com/shchmue/Lockpick_RCM), included with [Kosmos](https://github.com/AtlasNX/Kosmos), to dump your keys, then [hactool](https://github.com/SciresM/hactool) to extract the PFS0 AKA NSP and the NCAs it contains)

## Installation
1) Install [nx-ovlloader](https://github.com/WerWolv/nx-ovlloader/releases) and [Tesla-Menu](https://github.com/WerWolv/Tesla-Menu) (both are included with [Kosmos](https://github.com/AtlasNX/Kosmos) v15.2+)
2) Get ``MEGAHAKUS.ovl`` from Releases (or clone/download the repository and build it yourself) and copy it to ``switch/.overlays/`` (it's a hidden folder)

## No-NPR black skin workaround
1) Copy ``shader/hlsl/cloth_defaults.bnsh`` from the game romfs to ``atmosphere/contents/0100F3100DA46000/romfs/shader/hlsl/`` and rename it to ``glass_eyes.bnsh``
2) Copy ``shader/hlsl/glass_eye_nonssss.bnsh`` from the game romfs to ``atmosphere/contents/0100F3100DA46000/romfs/shader/hlsl/`` and rename it to ``skin_defaults.bnsh``

## Outlines in NPR-only songs
1) Copy ``pv_082.adp``, ``pv_243.adp``, ``pv_268.adp``, and ``pv_988.adp`` from ``romfs:/rom_switch/rom/add_param/`` to ``atmosphere/contents/0100F3100DA46000/romfs/rom_switch/rom/adp_force/``
2) To fix outlines in PS4 songs, you'll also need ``pv_234.adp``, ``pv_434.adp``, ``pv_610.adp``, and ``pv_629.adp`` in both ``atmosphere/contents/0100F3100DA46000/romfs/rom_switch/rom/adp_force/`` and ``atmosphere/contents/0100F3100DA46000/romfs/rom_switch/rom/add_param/``

Note that if you disable or enable NPR, you need to go back to the PV selector to apply your changes.

