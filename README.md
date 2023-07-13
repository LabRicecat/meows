# meows
My language collection

## Installation
This project requires the [catcaretaker](https://github.com/labricecat/catcaretaker) dependency manager.
```
$ git clone https://github.com/labricecat/meows
$ cd meows
$ catcare sync
$ mdkir build
$ cd build 
$ cmake .. && sudo make install
```
## Usage
```
meows <language> <files..>

language ::=
    -ccs -carescript       :  carescript
    -mnya -mathenyatica    :  mathenyatica
    -fnya -fnyactional     :  fnyactional
    -nyasm                 :  nyasm
    -snya -stack-nyachine  :  stack-nyachine

<files..> will get merged and fed to the interpreter or compiler

```