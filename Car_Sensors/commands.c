#include "commands.h"

#include "structs.h"

void exit_program(sensor *sensors, int number_of_sensors)
{
	for (int i = 0; i < number_of_sensors; i++) {
		free((sensors[i]).sensor_data);
		free((sensors[i]).operations_idxs);
	}
	free(sensors);
}

void print_sensor(sensor *sensors, int number_of_sensors, int index)
{
	if (index >= number_of_sensors || index < 0) {
		printf("Index not in range!\n");
		return;
	}
	if (sensors[index].sensor_type == TIRE) {
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n",
			   ((tire_sensor *)sensors[index].sensor_data)->pressure);
		printf("Temperature: %.2f\n",
			   ((tire_sensor *)sensors[index].sensor_data)->temperature);
		printf("Wear Level: %d%%\n",
				((tire_sensor *)sensors[index].sensor_data)->wear_level);
		if (((tire_sensor *)sensors[index].sensor_data)->performace_score != 0)
			printf("Performance Score: %d\n",
					((tire_sensor *)sensors[index].sensor_data)
					->performace_score);
		else
			printf("Performance Score: Not Calculated\n");
	} else if (sensors[index].sensor_type == PMU) {
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n",
			   ((power_management_unit *)sensors[index].sensor_data)->voltage);
		printf("Current: %.2f\n",
				((power_management_unit *)sensors[index].sensor_data)->current);
		printf("Power Consumption: %.2f\n",
				((power_management_unit *)sensors[index].sensor_data)
					->power_consumption);
		printf("Energy Regen: %d%%\n",
				((power_management_unit *)sensors[index].sensor_data)
					->energy_regen);
		printf("Energy Storage: %d%%\n",
				((power_management_unit *)sensors[index].sensor_data)
					->energy_storage);
	}
}

void delete_array(sensor **sensors, int *number_of_sensors, int index)
{
	// classic function that deletes an element from an array
	// by moving each element to the left starting with the
	// trash element's position + 1
	sensor trash = (*sensors)[index];
	for (int i = index; i < *number_of_sensors - 1; i++)
		(*sensors)[i] = (*sensors)[i + 1];
	free(trash.sensor_data);
	free(trash.operations_idxs);
	(*number_of_sensors)--;
	*sensors = realloc(*sensors, sizeof(sensor) * (*number_of_sensors));
	DIE(!*sensors, "Failed to malloc.\n");
}

void clear(sensor **sensors, int *number_of_sensors)
{
	// check the proprieties for each type of sensor
	for (int i = 0; i < *number_of_sensors; i++) {
		if ((*sensors)[i].sensor_type == TIRE) {
			if (((tire_sensor *)(*sensors)[i].sensor_data)->pressure > 28 ||
				((tire_sensor *)(*sensors)[i].sensor_data)->pressure < 19 ||
				((tire_sensor *)(*sensors)[i].sensor_data)->temperature > 120 ||
				((tire_sensor *)(*sensors)[i].sensor_data)->temperature < 0 ||
				((tire_sensor *)(*sensors)[i].sensor_data)->wear_level > 100 ||
				((tire_sensor *)(*sensors)[i].sensor_data)->wear_level < 0) {
				delete_array(sensors, number_of_sensors, i);
				i--;
			}
		}
		if ((*sensors)[i].sensor_type == PMU) {
			if (((power_management_unit *)(*sensors)[i].sensor_data)->voltage <
					10 ||
				((power_management_unit *)(*sensors)[i].sensor_data)->current <
					-100 ||
				((power_management_unit *)(*sensors)[i].sensor_data)->current >
					100 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->power_consumption > 1000 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->power_consumption < 0 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->energy_regen < 0 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->energy_regen > 100 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->energy_storage < 0 ||
				((power_management_unit *)(*sensors)[i].sensor_data)
						->energy_storage > 100) {
				delete_array(sensors, number_of_sensors, i);
				i--;
			}
		}
	}
}

void analyze(sensor *sensors, int index)
{
	// declare an array of functions type void
	void (*array_operations[8])(void *);  // we know that there are 8 operations

	// initialize the vector using the function get_operations
	get_operations((void *)array_operations);

	// call the right operations for the sensor
	for (int i = 0; i < sensors[index].nr_operations; i++) {
		int nr = sensors[index].operations_idxs[i];  // for easier writing
		array_operations[nr](sensors[index].sensor_data);
	}
}

void sort_priority(sensor **sensors, int number_of_sensors)
{
	sensor *tmp = malloc(number_of_sensors * sizeof(sensor));
	DIE(!tmp, "Failes to malloc\n");
	int index = 0;
	for (int i = 0; i < number_of_sensors; i++)
		if ((*sensors)[i].sensor_type == PMU) {
			tmp[index] = (*sensors)[i];
			index++;
		}
	for (int i = 0; i < number_of_sensors; i++)
		if ((*sensors)[i].sensor_type == TIRE) {
			tmp[index] = (*sensors)[i];
			index++;
		}
	// exit_program(*sensors, number_of_sensors);
	// *sensors = tmp;
	for (int i = 0; i < number_of_sensors; i++)
		(*sensors)[i] = tmp[i];
	free(tmp);
}

void program(sensor *sensors, int number_of_sensors)
{
	int index;
	char *command;
	while (1) {
	command = malloc(100 * sizeof(char));
		DIE(!command, "Failed to malloc.\n");
		scanf("%s", command);
		if (strcmp(command, "print") == 0) {
			scanf("%d", &index);
			print_sensor(sensors, number_of_sensors, index);
		} else if (strcmp(command, "clear") == 0) {
			clear(&sensors, &number_of_sensors);
		} else if (strcmp(command, "analyze") == 0) {
			scanf("%d", &index);
			analyze(sensors, index);
		} else if (strcmp(command, "exit") == 0) {
			exit_program(sensors, number_of_sensors);
			break;
		} else
		printf("Invalid command\n");
		free(command);
	}
	free(command);
}
