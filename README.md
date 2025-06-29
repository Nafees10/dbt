# DBT

Extremely lightweight
[Interception Tools](https://gitlab.com/interception/linux/tools) plugin for
DeBounce Time. Built because my keyboard likes to press keys ~7 times for every
one actual press.

## Building

Pre-requisites:

- `git`
- `gcc`
- `make`
- Interception Tools

Run the following to build and install:

```bash
git clone https://github.com/Nafees10/dbt.git
cd dbt
make
strip dbt
sudo cp dbt /usr/local/bin/
```

## Configuration

The only thing that is meant to be changeable is the debounce time. You should
see a line in `dbt.c` at top:

```c
#define DEB_TIME_MS 20
```

Change 20 to milliseconds of debounce time.

## Setting It Up

Below is an example udevmon config (`/etc/interception/udevmon.yaml`):

```yaml
- JOB: "intercept -g $DEVNODE | dbt | uinput -d $DEVNODE"
  DEVICE:
    EVENTS:
      EV_KEY: [KEY_W, KEY_A, KEY_S, KEY_D]
    NAME: ".*((k|K)(eyboard|EYBOARD)|).*"
```

After writing this file, execute the following as root:
```bash
sleep 2; systemctl restart udevmon.service
```

---

See also: [1806](https://github.com/Nafees10/1806)
