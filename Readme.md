<div align="center">
    <h1>SimpleModDownloader</h1>
    <p>A Switch homebrew that allows you to see upcoming games on the Switch</p>
    <a rel="IMAGE">
        <img src="./resources/img/icon.jpg">
    </a>
</div>

<p>

<p align="center">
    <a rel="LICENSE" href="https://github.com/PoloNX/SimpleModDownloader/blob/master/LICENSE">
        <img src="https://img.shields.io/static/v1?label=license&message=GPLV3&labelColor=111111&color=0057da&style=for-the-badge&logo=data%3Aimage/png%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAABQAAAATCAYAAACQjC21AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIGNIUk0AAHpFAACAgwAA/FcAAIDoAAB5FgAA8QEAADtfAAAcheDStWoAAAFGSURBVHjarJK9LgRhFIafWUuiEH/rJwrJClEq3IELUKgo3IrETWh0FC7BNVih0AoKBQoEydq11qMwm5yMsbPEm3yZd55zvnfO92VQKVhLak09UZeL%2BrsVZ9Qdv2tXnf1NYEndUushZFGthvemuq32FwWuq%2BeZid5DvZGpXambeYGr6qnd9dGldqaudQL3QuFWvVbbmaC6%2BprDr9WbwA4SdQW4BwaABb50CTykfjjwC%2BAx9SPAfOANYDxRCXpOnxNAM4ePA63Ul8NHR4E2QClsGgGG0jUR%2BFjglcAn8/pj4HTwUz/42FPJ68lOSDhCkR/O46XM0Qh3VcRH83jph%2BZefKUosBr8XA%2B%2BmufLAR4Dh6k/CrzWA691YOc/3Ejv6iNM3k59Xw%2B8D3gC9hN1ErjjfzSbqHVg8J8CG2XgBXgL4/9VCdD6HACaHdcHGCRMgQAAAABJRU5ErkJggg%3D%3D" alt=License>
    </a>
    <a rel="VERSION" href="https://github.com/PoloNX/UpcomingSwitchGames">
        <img src="https://img.shields.io/static/v1?label=version&message=1.0.0&labelColor=111111&color=06f&style=for-the-badge" alt="Version">
    </a>
</p>

---

- [Screenshot](#screenshot)
- [How to build](#how-to-build)
- [Help me](#help-me)
- [Credits](#credits)


## Screenshot

![](./screenshots/1.jpg)

<details>
  <summary><b>More Screenshots</b></summary>

![](./screenshots/2.jpg)
![](./screenshots/3.jpg)
![](./screenshots/4.jpg)

</details>

## How to build

### Requirements

- [cmake](https://cmake.org/)
- [devkitPro](https://devkitpro.org/wiki/Getting_Started)

### Build

```bash
git clone --recursive https://github.com/PoloNX/UpcomingSwitchGames/
cd UpcomingSwitchGames
cmake -B cmake-build-switch -G Ninja -DPLATFORM_SWITCH=ON -DCMAKE_BUILD_TYPE=Release 
cmake --build cmake-build-switch --target UpcomingSwitchGames.nro
```

## Help me

If you want to help me open an issue when you encounter a bug and a pull request if you have a fix.

## Credits 

- Thanks to [Natinusala](https://github.com/natinusala) and [xfangfang](https://github.com/xfangfang) for [borealis](https://github.com/xfangfang/borealis)
- Thanks to [xfangfang](https://github.com/xfangfang) for tour help with borealis
- Thanks to [blawar](https://github.com/blawar) for titleDB
