To pit or not to pit... this is the strategy

Car Sensors

The program manages to show information about cars, based on Tire and Power Management Unit sensors, from input files given as parametre in the command line.

The prohram can display information such as temperature, pressure, wear leverl, performance, power, regenarate energy, energy usage and health of the battery.

The PMU sensors have a higher priority than the Tire sensors, so based on the sensors given, the operations will display first the result of the PMU ones.

For this, I used a sort function, that puts the PMU sensors on the first place, but in the order given as input.

The program is based on commands. The commands are printing the sensors, analyze, which displays the information of the sensors, clear that deletes from the given sensors the ones with incorrect features and exit, command that stops the program.

The purpose of this project is working with structures and using pointers of functions from extern file.