#include <iostream>
#include <mpi.h>
#include <cmath>
#include <fstream>
using namespace std;

int* allocate(int length) {
    return (int*) malloc(length * sizeof(int*));
}

bool get_cell(int* field, int x, int y, int width) {
    return field[y * width + x] == 1;
}

bool set_cell(int* field, int x, int y, int width, bool alive) {
    return field[y * width + x] = alive ? 1 : 0;
}

int* read_field_part(ifstream& input_file, int field_size, int part_height) {
    int* field_part = allocate(part_height * field_size);
    for (int y = 0; y < part_height; y++) {
        string line;
        input_file >> line;
        for (int x = 0; x < field_size; x++) {
            set_cell(field_part, x, y, field_size, line[x] == '#');
        }
    }
    return field_part;
}

void send_top_row_up(int rank, int* field_part, int field_size) {
    if (rank != 0) {
        int* row_to_send = allocate(field_size);
        for (int x = 0; x < field_size; x++) {
            bool cell = get_cell(field_part, x, 0, field_size);
            set_cell(row_to_send, x, 0, field_size, cell);
        }
        MPI_Send(row_to_send, field_size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }
}

int* receive_bottom_row_from_below(int rank, int mpi_size, int field_size) {
    int* bottom_row = allocate(field_size);
    if (rank != mpi_size - 1) {
        MPI_Recv(bottom_row, field_size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        for (int x = 0; x < field_size; x++) {
            set_cell(bottom_row, x, 0, field_size, false);
        }
    }
    return bottom_row;
}

void send_bottom_row_down(int rank, int mpi_size, int* field_part, int field_size, int part_height) {
    if (rank != mpi_size - 1) {
        int* row_to_send = allocate(field_size);
        for (int x = 0; x < field_size; x++) {
            bool cell = get_cell(field_part, x, part_height - 1, field_size);
            set_cell(row_to_send, x, 0, field_size, cell);
        }
        MPI_Send(row_to_send, field_size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
}

int* receive_top_row_from_above(int rank, int field_size) {
    int* top_row = allocate(field_size);
    if (rank != 0) {
        MPI_Recv(top_row, field_size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        for (int x = 0; x < field_size; x++) {
            set_cell(top_row, x, 0, field_size, false);
        }
    }
    return top_row;
}

int get_alive_neighbours(int* field_part, int* top_row, int* bottom_row, int x, int y, int part_height, int field_size) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int neighbour_y = y + dy;
            int neighbour_x = x + dx;

            int* array = field_part;
            if (neighbour_y < 0) {
                array = top_row;
                neighbour_y = 0;
            } else if (neighbour_y >= part_height) {
                array = bottom_row;
                neighbour_y = 0;
            }

            if (neighbour_x < 0 || neighbour_x >= field_size) {
                continue;
            }

            bool neighbour_alive = get_cell(array, neighbour_x, neighbour_y, field_size);
            if (neighbour_alive) {
                count++;
            }
        }
    }
    return count;
}

int* update_cells(int* field_part, int* top_row, int* bottom_row, int part_height, int field_size) {
    int* new_field_part = allocate(part_height * field_size);
    for (int y = 0; y < part_height; y++) {
        for (int x = 0; x < field_size; x++) {
            int alive_neighbours = get_alive_neighbours(field_part, top_row, bottom_row, x, y, part_height, field_size);
            bool cell_alive = get_cell(field_part, x, y, field_size);
            if (!cell_alive && alive_neighbours == 3) {
                set_cell(new_field_part, x, y, field_size, true);
            } else if (cell_alive && (alive_neighbours < 2 || alive_neighbours > 3)) {
                set_cell(new_field_part, x, y, field_size, false);
            } else {
                set_cell(new_field_part, x, y, field_size, cell_alive);
            }
        }
    }
    return new_field_part;
}

void write_field_part(ofstream& output_file, int* field_part, int field_size, int part_height) {
    for (int y = 0; y < part_height; y++) {
        for (int x = 0; x < field_size; x++) {
            bool cell_alive = get_cell(field_part, x, y, field_size);
            output_file << (cell_alive ? '#' : '.');
        }
        output_file << endl;
    }
}

int main_mpi(int argc, char **argv, int rank, int mpi_size) {
    int steps_count;
    int field_size;
    int part_height;
    int field_part_size;
    int* field_part;

    if (rank == 0) {
        if (argc < 3) {
            cout << "Not enough arguments!" << endl;
            cout << "Format: " << argv[0] << " input_file_name output_file_name" << endl;
            return 1;
        }

        char* input_file_name = argv[1];
        ifstream input_file(input_file_name);
        if (!input_file) {
            cout << "Unable to open input file with name " << input_file_name;
            return 2;
        }

        input_file >> steps_count;
        input_file >> field_size;
        part_height = field_size / mpi_size;
        for (int i = 1; i < mpi_size; i++) {
            MPI_Send(&steps_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&field_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&part_height, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        if (mpi_size > field_size) {
            cout << "field_size " << field_size << " is too small (less then MPI_Comm_size)";
            return 3;
        }

        if (field_size % mpi_size != 0) {
            cout << "MPI_Comm_size must divide field_size without remainder";
            return 4;
        }

        field_part_size = part_height * field_size;
        field_part = read_field_part(input_file, field_size, part_height);
        for (int i = 1; i < mpi_size; i++) {
            int* field_part_to_send = read_field_part(input_file, field_size, part_height);
            MPI_Send(field_part_to_send, field_part_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&steps_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&field_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&part_height, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        field_part_size = part_height * field_size;
        field_part = allocate(field_part_size);
        MPI_Recv(field_part, field_part_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int step = 0; step < steps_count; step++) {
        send_top_row_up(rank, field_part, field_size);
        int* bottom_row = receive_bottom_row_from_below(rank, mpi_size, field_size);
        send_bottom_row_down(rank, mpi_size, field_part, field_size, part_height);
        int* top_row = receive_top_row_from_above(rank, field_size);
        field_part = update_cells(field_part, top_row, bottom_row, part_height, field_size);
    }

    if (rank == 0) {
        char* output_file_name = argv[2];
        ofstream output_file(output_file_name);
        write_field_part(output_file, field_part, field_size, part_height);
        for (int i = 1; i < mpi_size; i++) {
            int* other_field_part = allocate(field_part_size);
            MPI_Recv(other_field_part, field_part_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_field_part(output_file, other_field_part, field_size, part_height);
        }
    } else {
        MPI_Send(field_part, field_part_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}

int main(int argc, char **argv) {
    MPI_Init (&argc, &argv);
    int rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    double start = MPI_Wtime();
    int return_code = main_mpi(argc, argv, rank, mpi_size);
    double end = MPI_Wtime();

    if (rank == 0) {
        cout << "Completed in " << end - start << "s" << endl;
    }

    MPI_Finalize();
    return return_code;
}