# Grand Prix Legends Season Manager

[![Windows (MSVC)](https://github.com/casqade/gplSeasonManager/actions/workflows/windows-build-msvc.yml/badge.svg)](https://github.com/casqade/gplSeasonManager/actions/workflows/windows-build-msvc.yml)
[![Windows (MSYS2)](https://github.com/casqade/gplSeasonManager/actions/workflows/windows-build-msys2.yml/badge.svg)](https://github.com/casqade/gplSeasonManager/actions/workflows/windows-build-msys2.yml)
[![GitHub Releases](https://img.shields.io/github/release/casqade/gplSeasonManager.svg)](https://github.com/casqade/gplSeasonManager/releases/latest)

---

This little utility automates enabling & disabling tracks
in Grand Prix Legends, the best retro racing sim to this day.


## Help Entry

```
Usage: gplSeasonManager [options] <command> [<args>]

Examples:
  gplSeasonManager enable rouen spa67
  gplSeasonManager disable mexico silver
  gplSeasonManager -s=customSeason.ini enable monza10k nepliget
  gplSeasonManager --season=2024season.ini list


Commands:
  enable  [track...]    Add one or more tracks to the season
  disable [track...]    Remove one or more tracks from the season
  restore               Rewrite season with original 67season.ini contents
  list                  List currently enabled tracks in a season


Options:
  -h --help             Show this help

  -s=FILENAME           Use specified file for operations
  --season=FILENAME     instead of default 67season.ini
                        (File must be in seasons/ subdirectory)
```


## FAQ

### How to enable a track

1. Place `gplSeasonManager.exe` in Grand Prix Legends root directory, 
right next to `gpl.exe`.
2. Let's pretend you want to enable `spa67`, 
which is already present in `tracks/` subdirectory.
3. Download the corresponding
[`event.ini`](https://github.com/Casqade/gplEventsDb/blob/main/events/Spa-Francorchamps/spa67/event.ini) 
file from [GPL Events Database](https://github.com/Casqade/gplEventsDb) and place it inside `spa67` directory. 
4. Open Command Line in GPL directory & execute 
the following command command: `gplSeasonManager enable spa67`
5. If no errors occurred, the track is now enabled.

By default, this program reads & modifies `67season.ini`. 
If you want to edit other seasons, please refer to [Usage Examples](https://github.com/Casqade/gplSeasonManager#help-entry).


### Why not GEM+?

1. I hate non-portable software. 
2. I hate software doing magic under the hood and 
patching my executables & files without notice. 
3. GEM+ uses `sgem.ini` to enable tracks in a similar manner, 
but the latest official GEM+ package was released in 2010,
meaning in 2024 this file is really outdated and doesn't include 
information about newer tracks. Updated `sgem.ini` versions
aren't publicly available and only registered users can access these files. 
