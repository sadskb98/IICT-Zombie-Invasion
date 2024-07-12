####################################################################################################################
##                                                                                                                ##
##      )            )    )     *           (          (        )                  (    (        )      )    ____ ##
##   ( /(         ( /( ( /(   (  `      (   )\ )       )\ )  ( /(           (      )\ ) )\ )  ( /(   ( /(   |   / ##
##   )\())   (    )\()))\())  )\))(   ( )\ (()/( (    (()/(  )\()) (   (    )\    (()/((()/(  )\())  )\())  |  /  ##
## |((_)\    )\  ((_)\((_)\  ((_)()\  )((_) /(_)))\    /(_))((_)\  )\  )\((((_)(   /(_))/(_))((_)\  ((_)\   | /   ##
## |_ ((_)_ ((_)  _((_) ((_) (_()((_)((_)_ (_)) ((_)  (_))   _((_)((_)((_))\ _ )\ (_)) (_))    ((_)  _((_)  |/    ##
## | |/ /| | | | |_  / / _ \ |  \/  | | _ )|_ _|| __| |_ _| | \| |\ \ / / (_)_\(_)/ __||_ _|  / _ \ | \| | (      ##
##   ' < | |_| |  / / | (_) || |\/| | | _ \ | | | _|   | |  | .` | \ V /   / _ \  \__ \ | |  | (_) || .` | )\     ##
##  _|\_\ \___/  /___| \___/ |_|  |_| |___/|___||___| |___| |_|\_|  \_/   /_/ \_\ |___/|___|  \___/ |_|\_|((_)    ##
##                                                                                                                ##
####################################################################################################################
####################################################################################################################

play:
	./iict_zombie

install:
	sudo apt install build-essential -y
	sudo apt-get install libsdl1.2-dev -y
	sudo apt-get install libsdl-image1.2-dev -y
	sudo apt-get install libsdl-mixer1.2-dev -y

build:
	gcc -o ku_zombie main.cpp -lSDL -lSDL_image -lSDL_mixer -lm

