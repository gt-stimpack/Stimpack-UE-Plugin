# Stimpack: Unreal Engine Plugin & Game Environments

[![Paper](https://img.shields.io/badge/Paper-PDF-red)](https://www.usenix.org/system/files/nsdi26-heo.pdf)
[![Unreal Engine](https://img.shields.io/badge/Unreal_Engine-5.0.3-0E1128?logo=unrealengine&logoColor=white)](https://www.unrealengine.com/)
[![C++](https://img.shields.io/badge/Language-C++-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![ZeroMQ](https://img.shields.io/badge/IPC-ZeroMQ-DF0000?logo=zeromq&logoColor=white)](https://zeromq.org/)

This repository contains the **Unreal Engine 5 (UE5) source code and Stimpack plugins** for the games used in the Stimpack research paper. It acts as the "End-point" that communicates with the [Stimpack Central Runtime](https://github.com/gt-stimpack/Stimpack) to adaptively optimize rendering quality.

### 🏗️ Repository Structure
The repository is organized into three separate Unreal Engine projects, each representing a different game environment:
* **Game1, Game2, Game3**: Individual UE project folders containing source code, ZeroMQ plugins, and configuration files.
* **Plugins/ZeroMQ**: The C++ implementation of the Stimpack-Runtime communication bridge.

```
.
├── Game1
│   ├── Config         # Contains userinfo.txt and emul.txt for runtime and emulation modes
│   ├── Content        # 3D assets (not included due to size, must be downloaded from Marketplace)
│   ├── Game1.uproject
│   ├── Plugins        # Contains the Stimpack plugin for ZeroMQ communication
│   └── Source         # C++ source code for the game and Stimpack plugin
├── Game2
│   ├── Config         # Contains userinfo.txt and emul.txt for runtime and emulation modes
│   ├── Content        # 3D assets (not included due to size, must be downloaded from Marketplace)
│   ├── Game2.uproject
│   ├── Plugins        # Contains the Stimpack plugin for ZeroMQ communication
│   └── Source         # C++ source code for the game and Stimpack plugin
├── Game3
│   ├── Config         # Contains userinfo.txt and emul.txt for runtime and emulation modes
│   ├── Content        # 3D assets (not included due to size, must be downloaded from Marketplace)
│   ├── Game3.uproject
│   ├── Plugins        # Contains the Stimpack plugin for ZeroMQ communication
│   └── Source         # C++ source code for the game and Stimpack plugin
├── LICENSE
└── README.md
```

---

### 🛠️ Prerequisites & Setup

#### 1. Environment
* **Unreal Engine:** v5.0.3 (Tested on Ubuntu 22.04)
* **Compiler:** Clang (Standard for UE Linux build)
* **Dependencies:** Stimpack uses ZeroMQ for IPC. The plugin is included, but ensure your system supports ZMQ libraries.
> **Dependencies:** Stimpack uses ZeroMQ for IPC. While the plugin source is included, you may need to install the ZMQ library on your Linux system:
> ```bash
> sudo apt-get update && sudo apt-get install -y libzmq3-dev
> ```

#### 2. Assets (Critical!)
Due to file size constraints (several GBs), **3D assets (Content folder) are not included in this repository.** You must download them from the Unreal Engine Marketplace and move them manually:
1. **Identify Assets:** Refer to the links below for the required Marketplace assets for each game.
   * [Game 1 Asset Link](https://www.unrealengine.com/marketplace/en-US/item/be025cd5631141868361885b169e9994)
   * [Game 2 Asset Link](https://www.unrealengine.com/marketplace/en-US/item/f0f978cdc78348b5b5d0b1c5a11e6d53)
   * [Game 3 Asset Link](https://www.unrealengine.com/marketplace/en-US/item/96a5ab5b99ef4c1ab36a1bbc5a36074a)
2. **Download:** Use the **Epic Games Launcher** (Windows or macOS) to download the assets.
3. **Migrate:** Copy the `Content` folder from your Windows/Mac machine to the respective project folder on your Linux build machine (e.g., `Stimpack-UE-Plugin/Game1/Content`).

---

### ⚙️ Configuration

Each game project contains two key configuration files in the `Config/` directory:

| File | Mode | Description |
| :--- | :--- | :--- |
| **`userinfo.txt`** | **Live Mode** | Defines Codec, initial QP, Frame Latency SLO (Target FPS), and FPS Upper Bound. Used when connecting to the Stimpack Runtime. |
| **`emul.txt`** | **Emulation Mode** | Defines pre-set RQ/FPS state changes. If this file exists, the game runs locally without the Stimpack Runtime for testing purposes. |

**Note:** By default, the plugin attempts to connect to the runtime at `localhost:10000` for initial port negotiation.

---

### 🚀 How to Run

#### 1. Building the Game
1. Open the `.uproject` file in the **Unreal Editor**.
2. If prompted to rebuild missing modules (ZeroMQ/GameSource), click **Yes**.
3. Go to **Platforms > Linux** and select **Package Project**.

#### 2. Execution
Once packaged, ensure the `userinfo.txt` (and `emul.txt` if needed) are present in the packaged `Config` folder.

**To run with Stimpack Runtime (Live Mode):**
1. Start the [Stimpack Runtime](https://github.com/gt-stimpack/Stimpack) first.
2. Launch the game executable:
   ```bash
   ./Game1.sh
   ```
> [!IMPORTANT]
> **Config File Packaging:** Unreal Engine might not include `.txt` files in the packaged build by default. After packaging, manually copy your `userinfo.txt` and `emul.txt` into the `[Packaged_Project]/Game1/Config/` directory.
---

### Citation
```bibtex
@inproceedings{heo2026stimpack,
  title={Stimpack: An Adaptive Rendering Optimization System for Scalable Cloud Gaming},
  author={Heo, Jin and Wang, Vic and Bhardwaj, Ketan and Gavrilovska, Ada},
  booktitle={23rd USENIX Symposium on Networked Systems Design and Implementation (NSDI 26)},
  pages={397--414},
  year={2026}
}
```

### License
- Stimpack plugin codes licensed under the [MIT License](LICENSE).
- The used 3D assets are licensed under their respective Unreal Engine Marketplace licenses.
  - [FANTASTIC - Village Pack](unrealengine.com/marketplace/en-US/item/be025cd5631141868361885b169e9994): [Fab Standard License](https://www.fab.com/eula).
  - [Stylized Fantasy Provencal](https://www.fab.com/listings/ced19ea1-31ed-437f-ae64-2b6b1561fede): [Fab Standard License](https://www.fab.com/eula).
  - [Medieval Houses Modular Vol 2](https://www.fab.com/listings/a84ffa65-2294-4721-bbec-9f876550c6ad): [Fab Standard License](https://www.fab.com/eula).

---

### 🕹️ Fun Fact: Why the name "Stimpack"?

The name **Stimpack** is a tribute to the iconic ability of the **Terran Marine** in Blizzard's *StarCraft* series.

In the game, a Marine injects a Stimpack to trade a bit of health for a massive boost in combat speed and efficiency.
Similarly, our system strategically adjusts (or "sacrifices") unnecessary rendering details to significantly boost the overall **scalability and utility** of the cloud gaming server.

As a former competitive *StarCraft* player, the lead author designed Stimpack with a "gamer-first" mindset: ensuring that every bit of computing power is spent where it actually improves the experience.

<p align="center">
  <img src="https://github.com/gt-stimpack/Stimpack/blob/main/figs/marines.gif" width="400" alt="StarCraft Marine Stimpack" />
</p>

---
