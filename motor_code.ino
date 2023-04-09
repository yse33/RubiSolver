const int step_pin_one = 3;
const int dir_pin_one = 4;

const int step_pin_two = 5;
const int dir_pin_two = 6;

const int step_pin_three = 7;
const int dir_pin_three = 8;

int step = 50;
int rotation_offset = 10;
int const motor_speed = 750;
int const wait = 750;

void rotate(int step_pin, int given_step);
void rotate_cube();
void rotate_cube_helper();
void rotate_left_side(int direction, int m);
void rotate_right_side(int direction, int m);
void rotate_third_motor(int direction, int m);
void orient_cube(char current_face, char back_face);

void setup() {
    Serial.begin(9600);

    pinMode(step_pin_one, OUTPUT);
    pinMode(dir_pin_one, OUTPUT);

    pinMode(step_pin_two, OUTPUT);
    pinMode(dir_pin_two, OUTPUT);

    pinMode(step_pin_three, OUTPUT);
    pinMode(dir_pin_three, OUTPUT);

    char solution[100] = "";
    char eof = 'x';
    char c = ' ';
    char current_face = 'F';
    int solution_length = 0;
    int multiplier = 1;

    while (c != eof) {
        if (Serial.available() > 0) {
            c = Serial.read();
            strncat(solution, &c, 1);  
            solution_length++;          
        }
    }
    Serial.println("Received");


    solution[solution_length + 1] = '\0';

    for (int i = 0; i < solution_length; i++) {
        if (solution[i] == eof) {
            break;
        }

        if (solution[i] == 2) {
            continue;
        }

        multiplier = 1;

        if (solution[i + 1] == '2') {
            multiplier = 2;
        }
        Serial.println(solution[i]);
        switch (solution[i]) {
            case 'L':
                rotate_left_side(1, multiplier);
                break;
            case 'l':
                if (multiplier == 2) {
                    rotate_left_side(1, multiplier);
                } else {
                    rotate_left_side(1, 3);
                }
                break;
            case 'R':
                if (multiplier == 2) {
                    rotate_right_side(1, multiplier);
                } else {
                    rotate_right_side(1, 3);
                }
                break;
            case 'r':
                rotate_right_side(1, multiplier);
                break;
            case 'B':
                orient_cube(current_face, 'B');
                rotate_third_motor(0, multiplier);
                current_face = 'F';
                break;
            case 'b':
                orient_cube(current_face, 'B');
                rotate_third_motor(1, multiplier);
                current_face = 'F';
                break;
            case 'U':
                orient_cube(current_face, 'U');
                rotate_third_motor(0, multiplier);
                current_face = 'D';
                break;
            case 'u':
                orient_cube(current_face, 'U');
                rotate_third_motor(1, multiplier);
                current_face = 'D';
                break;
            case 'F':
                orient_cube(current_face, 'F');
                rotate_third_motor(0, multiplier);
                current_face = 'B';
                break;
            case 'f':
                orient_cube(current_face, 'F');
                rotate_third_motor(1, multiplier);
                current_face = 'B';
                break;
            case 'D':
                orient_cube(current_face, 'D');
                rotate_third_motor(0, multiplier);
                current_face = 'U';
                break;
            case 'd':
                orient_cube(current_face, 'D');
                rotate_third_motor(1, multiplier);
                current_face = 'U';
                break;
        }
    }
}

void loop() {
    //pass
}

void rotate(int step_pin, int given_step, int speed) {
    for (int i = 0; i < given_step; i++) {
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(speed);

        digitalWrite(step_pin, LOW);
        delayMicroseconds(speed);
    }

    delay(wait);
}

void rotate_cube() {
    digitalWrite(dir_pin_one, LOW);
    digitalWrite(dir_pin_two, HIGH);

    for (int i = 0; i < step + rotation_offset; i++) {
        digitalWrite(step_pin_one, HIGH);
        digitalWrite(step_pin_two, HIGH);
        delayMicroseconds(motor_speed);

        digitalWrite(step_pin_one, LOW);
        digitalWrite(step_pin_two, LOW);
        delayMicroseconds(motor_speed);
    }
}

void rotate_cube_helper() {
    digitalWrite(dir_pin_one, HIGH);
    digitalWrite(dir_pin_two, LOW);

    for (int i = 0; i < rotation_offset; i++) {
        digitalWrite(step_pin_one, HIGH);
        digitalWrite(step_pin_two, HIGH);
        delayMicroseconds(1500);
        
        digitalWrite(step_pin_one, LOW);
        digitalWrite(step_pin_two, LOW);
        delayMicroseconds(1500);
    }
}

void rotate_left_side(int direction, int m) {
    if (1 == direction) {
        digitalWrite(dir_pin_two, LOW);
    } else {
        digitalWrite(dir_pin_two, HIGH);
    }
    
    rotate(step_pin_two, step * m, motor_speed);
}

void rotate_right_side(int direction, int m) {
    if (1 == direction) {
        digitalWrite(dir_pin_one, HIGH);
    } else {
        digitalWrite(dir_pin_one, LOW);
    }
    
    rotate(step_pin_one, step * m, motor_speed);
}

void rotate_third_motor(int direction, int m) {
    for (int i = 0; i < m; i++) {
        if (1 == direction) {
            digitalWrite(dir_pin_three, HIGH);
        } else {
            digitalWrite(dir_pin_three, LOW);
        }
        
        rotate(step_pin_three, step + 20, 1250);

        if (1 == direction) {
            digitalWrite(dir_pin_three, LOW);
        } else {
            digitalWrite(dir_pin_three, HIGH);
        }

        rotate(step_pin_three, 20, 1250);
    }
}

void orient_cube(char current_face, char back_face) {
    int multiplier = 0;
    if ((current_face == 'F' && back_face == 'U') ||
        (current_face == 'U' && back_face == 'B') ||
        (current_face == 'B' && back_face == 'D') ||
        (current_face == 'D' && back_face == 'F')) 
        {
            multiplier = 1;
        } 
    else if((current_face == 'F' && back_face == 'F') ||
            (current_face == 'U' && back_face == 'U') ||
            (current_face == 'B' && back_face == 'B') ||
            (current_face == 'D' && back_face == 'D')) {
                multiplier = 2;
            }
    else if((current_face == 'F' && back_face == 'D') ||
            (current_face == 'D' && back_face == 'B') ||
            (current_face == 'B' && back_face == 'U') ||
            (current_face == 'U' && back_face == 'F')) {
                multiplier = 3;
            }

    Serial.print(current_face);
    Serial.print(back_face);
    Serial.println(multiplier);


    for (int i = 0; i < multiplier; i++) {
        rotate_cube();
        delay(motor_speed);
        rotate_cube_helper();
        delay(motor_speed);
    }
    
    delay(wait);
}
