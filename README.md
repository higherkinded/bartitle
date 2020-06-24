## What is this?

It's a simple statusbar I've wrote for use with DWM. I didn't feel like calling
to anything too resource-hungry to do it and I also felt compelled to write some
C to fulfill my needs.

## How to use it?

First off, modify it to whatever point you want, I don't mind, it's MIT and I
understand perfectly well that you can write your own and do it better.

It must be also mentioned that `BATTERY` build option relies on `sys` FS, so
the feature is Linux only. I love BSDs and all but it was written on a target
machine and I'll probably port it somewhere in the future.

### Building

In any case, when you build it, you'll end up with `bin/bartitle` in `dist`
directory. Copy it over to `/usr/local/bin` or use it right away.

The basic build is no-bells-and-whistles. It just results in a bar with a clock
and formats it to `BAR_TEMPLATE`:

```bash
make
```

It can also be built with a makeopt to enable certain features. For now it's
just an optional battery support, which you can enable if you build with:

```bash
make BATTERY=BATTERY_NAME_IN_SYSFS
```

Other compile-time options:

- `BATTERY_POLL_TICK` - defines how frequent the battery check is. By default it
    is set to 5, which makes the battery routine run every 5 ticks where one
    tick is 0.25s by default.

You can look up the name in `/sys/class/power_supply`.

### Running

Once you've got your hands on binary, you can run it:

```bash
bartitle # Or ./dist/bin/bartitle if you didn't copy it over
```

This will run it without disowning. If you want to disown it though, you'd do it
like this:

```bash
bartitle -d
```

## Why is it written so poorly?

IDK, I don't even know enough C to write it in a "proper" manner, so I just used
whatever I know about it in combination with manpages and libX11 source. It was
fun and I'll probably extend it if I'll feel like I'd like to. But for now, I
don't even know the definition of "poor C" well enough to judge my stuff. :/
