motor_size = 8.45;
motor_height = 10;
motor_wall_d = 2.2;
motor_clamp_cut = 3;

motor_helper_disc = 40;
motor_helper_height = 0.2;

motor_arm_width = 8;
motor_arm_height = 6;
motor_arm_length = 20;

motor_body_len = 40;
motor_body_height = 8;
motor_body_wall = 2;

triangle_cut_height = 8;
cable_cut_width = 3;
cable_cut_height = 1.5;

motor_body_arm_dist = (motor_body_len - motor_arm_width + 2) / sqrt(2);

fc_width = 20;
fc_height = 28;
rx_width = 12;
rx_height = 25;

$fn = 25;

module triangle(o_len, a_len, depth) {
    linear_extrude(height=depth)
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
}

module fc_pcb() {
    %cube([fc_width, fc_height, 2]);
}

module rx_pcb() {
    %cube([rx_width, rx_height, 2]);
}

module motor_clamp() {
    // motor / prop mockup
    %cylinder(d = motor_size, h = motor_height + 5);
    %cylinder(d = 2, h = motor_height + 15);
    %translate([0, 0, 20]) cylinder(d = 65, h = 2);
    
    difference() {
        union () {
            color("blue")
            hull() {
                cylinder(d = motor_size + motor_wall_d, h = motor_height);
                
                translate([-motor_arm_width / 2, -(motor_size + motor_wall_d) / 2 - 2, motor_height - motor_arm_height])
                    cube([motor_arm_width, 1, motor_arm_height]);
            }
            
            color("green")
            translate([0, 0, motor_height - motor_helper_height])
                cylinder(d = motor_helper_disc, h = motor_helper_height);
            
            color("blue")
            sphere(d = motor_size + motor_wall_d);
        }
        
        // cable hole
        translate([-cable_cut_width / 2, -(motor_size + motor_wall_d) / 2 - cable_cut_height, -5])
            cube([cable_cut_width, cable_cut_height, 20]);
        
        // motor hole
        translate([0, 0, -1])
            cylinder(d = motor_size, h = motor_height + 2);
        
        // clamp cutout
        translate([-motor_clamp_cut / 2, -3, -motor_size - motor_wall_d - 1])
            cube([motor_clamp_cut, motor_size + motor_wall_d + 5, motor_height + motor_size + motor_wall_d + 2]);
        
        sphere(d = motor_size);
    }
}

module arm() {
    motor_clamp();
    
    // motor arm itself
    color("blue")
        translate([-motor_arm_width / 2, -(motor_arm_length + ((motor_size + motor_wall_d) / 2) + 2), motor_height - motor_arm_height])
        cube([motor_arm_width, motor_arm_length, motor_arm_height]);
}

module body() {
    // arms
    for(r = [45 : 90 : 360]) {
        rotate([0, 0, r])
        translate([0, motor_arm_length + ((motor_size + motor_wall_d) / 2) + motor_body_arm_dist + 1, 0])
        difference() {
            arm();
            
            translate([-cable_cut_width / 2, -motor_arm_length - ((motor_size + motor_wall_d) / 2) - 2, motor_height - cable_cut_height])
                cube([cable_cut_width, motor_arm_length + 1, cable_cut_height + 1]);
        }
    }
    
    color("red")
    translate([-motor_body_len / 2, -motor_body_len / 2, motor_height - motor_body_height])
    difference() {
        cube([motor_body_len, motor_body_len, motor_body_height]);
        
        for(r = [45 : 90 : 360]) {
            translate([motor_body_len / 2, motor_body_len / 2, -motor_height + motor_body_height])
                rotate([0, 0, r])
                translate([-cable_cut_width / 2, motor_body_len / 2 * sqrt(2) - 6, motor_height - cable_cut_height]) {
                    cube([cable_cut_width, 8, cable_cut_height + 1]);
                    translate([0, -1, -5])
                        cube([cable_cut_width, cable_cut_height, 10]);
                }
        }
    }
}

module quad() {
    difference() {
        translate([0, 0, motor_height])
        rotate([180, 0, 0]) {
            body();
            
            translate([-3, -14, 6])
                fc_pcb();
            
            translate([-16, -12.5, 6])
                rx_pcb();
        }
        
        // cut out for actual PCBs
        translate([-(motor_body_len - (2 * motor_body_wall)) / 2, -(motor_body_len - (2 * motor_body_wall)) / 2, motor_body_wall])
            cube([motor_body_len - (2 * motor_body_wall), motor_body_len - (2 * motor_body_wall), motor_body_height - motor_body_wall + 1]);
        
        // usb cutout
        translate([-2, -motor_body_len / 2 - 1, 2])
            cube([12, 4, 7]);
        
        // triangle cut-outs
        translate([0, -motor_body_len / 3, -1]) {
            triangle(triangle_cut_height, motor_body_len / 4, 4);
            triangle(triangle_cut_height, -motor_body_len / 4, 4);
        }
        translate([0, motor_body_len / 3, -1]) {
            triangle(-triangle_cut_height, motor_body_len / 4, 4);
            triangle(-triangle_cut_height, -motor_body_len / 4, 4);
        }
        translate([-motor_body_len / 3, 0, -1])
        rotate([0, 0, -90]) {
            triangle(triangle_cut_height, motor_body_len / 4, 4);
            triangle(triangle_cut_height, -motor_body_len / 4, 4);
        }
        translate([motor_body_len / 3, 0, -1])
        rotate([0, 0, 90]) {
            triangle(triangle_cut_height, motor_body_len / 4, 4);
            triangle(triangle_cut_height, -motor_body_len / 4, 4);
        }
    }
}

quad();
