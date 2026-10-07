# PonyoCAM

> For my girlfriend (and best friend), so she can monitor her chinchillas safely away from home.

**⚠️ Early stage:** This is a personal project under active development. Things will change, break, and be rewritten.

---

## Why?

My girlfriend has chinchillas and they're super cute, but they're unfortunately vulnerable to high heat. Although she has them in a climate-controlled room, it's still scary to leave them alone for a while.

With this project, I want to create a networked camera with temperature sensing so that she can monitor them anytime she wants, even away from her home network.

Although plenty of off-the-shelf parts exist for IP cameras and temperature sensors with easy Internet access, I want to design the electronics myself as a learning opportunity and to have full control of my hardware and data. Relying on closed-source hardware and having our data processed through some random cloud server is undesirable, at least for me.

Through this project, I will be learning more about Python, web hosting, networking, Bluetooth LE, low power design, 3D CAD, and ECAD.

## Objectives

- [ ] Primary goal: Deploy a network camera and temperature sensor that is stable and accessible over the Internet
- [ ] Primary goal: Host a simple HTTP web server to serve temp sensor readings, so that an iOS custom widget app can display the data 
- [ ] Secondary goal: Design a PCB for temperature sensor and achieve 1+ year battery life off coin cell
- [ ] Stretch goal: Motion detection, image snapshots, network video recording (NVR)
### Non-Goals

Things this project deliberately will **not** do (helps scope and sets expectations; though maybe in the far future I could work on them).

- Computer vision to recognize a specific chinchilla and if they are eating, peeing, etc.

## Feature Status

| Feature            | State       | Notes                    |
|-----------------|-------------|--------------------------|
| IP Camera       | 🟢 Working | Enclosure and software not fully polished yet, but camera network stream is fully working |
| Temperature Sensor over BLE | 🟡 In progress  | Can get fully working easily, but want the Nordic PPK2 to accurately measure power usage |
| HTTP Server     | 🟡 In progress | Tailscale and ACLs are setup, need to create Docker container for server |                    |

## Roadmap

- [x] Milestone 0: Proof of concept
- [x] Milestone 1: Basic, working prototype of camera + temp sensor
- [ ] Milestone 2: Working prototype of new BLE-based BME280 sensor module
- [ ] Milestone 3: Temperature reading through iOS widget
- [ ] Milestone 4: Polish and docs

## Design Notes

### Camera System
- Raspberry Pi Zero W: already on hand, GPIOs, extremely small form factor, Linux, dedicated camera interface, and Wi-Fi/Bluetooth capabilities
  - No significant bottleneck found yet despite the Zero W's low performance; may need to upgrade if I decide to do an NVR
- OV5647 camera with noIR filter and IR LEDs for night vision: cheap and decent video quality, but daylight video has a purplish hue due to noIR
  - Could use a camera with a IR-cut filter that only removes IR filter in low-light, but not really needed right now
- Python and MediaMTX for simple camera network streaming
  - MediaMTX was initially used to test camera and network video, but it seems to work very well for now; no reason to switch to something else
 
### Temperature Sensor
- Adafruit BME280: already on hand, breakout board, measures ambient temperature accurately enough
- Initial case design had the BME280 in the same enclosure as the Pi and the camera, leading to very high temperature readings
- Next design will move to a Seeed Studio XIAO nRF52840 and Bluetooth LE for long-term and low-power wireless sensing, so that it can located far from any heat generating sources

### HTTP Server
- Still very WIP, but will probably just be place for the sensor to POST data and have apps GET the readings
- Still considering if server should be accessible publicly or only through Tailscale
- Will be hosted on homelab server to centralize services and since it has Cloudflare Tunnel already set up, in case I want to go the public route
- Homelab is running TrueNAS, so a Docker container will probably be the best basis for the HTTP server

### Custom PCB
- Still very WIP
- Planning to use a 3V lithium coin cell, so I don't need the onboard LDOs found on the Adafruit BME280 and the XIAO nRF52840
- Need to do more research on low power design
