#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "operations.h"
#include <errno.h>

void exit_program(sensor *sensors, int number_of_sensors);
void print_sensor(sensor *sensors, int number_of_sensors, int index);
void clear(sensor **sensors, int *number_of_sensors);
void analyze(sensor *sensors, int index);
void delete_array(sensor **sensors, int *number_of_sensors, int index);
void sort_priority(sensor **sensors, int number_of_sensors);
void program(sensor *sensors, int number_of_sensors);
