$fn = 25;
with_props=0;
with_motor=0;
with_pcbs=1;
with_akku=0;

motor_size = 8.465;
motor_height = 21;
motor_wall_d = 2.2;
motor_clamp_cut = 3;

usb_width=12;
usb_hight=8;

motor_helper_disc = 40;
motor_helper_height = 0.2;

motor_arm_width = 8;
motor_arm_height = 6;
motor_arm_length = 20;

motor_body_len = 40;
motor_body_height = 12;
motor_body_wall = 2;

triangle_cut_height = 10;
cable_cut_width = 3;
cable_cut_height = 3;

akku_holder_hight = 9.1;
akku_holder_width =27;
motor_body_arm_dist = (motor_body_len - motor_arm_width + 2) / sqrt(2);
module aligned_cube(size, aligned=[1,1,0])
{
    translate(-0.5*[size[0]*aligned[0], size[1]*aligned[1], size[2]*aligned[2]])
        cube(size);
}

module triangle(o_len, a_len, depth) {
    linear_extrude(height=depth)
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
}
module triangle_cuts(h=4) {
    translate([0, -motor_body_len / 3, 0]) {
        triangle(triangle_cut_height, motor_body_len / 4, h);
        triangle(triangle_cut_height, -motor_body_len / 4, h);
    }
    translate([0, motor_body_len / 3, 0]) {
        triangle(-triangle_cut_height, motor_body_len / 4, h);
        triangle(-triangle_cut_height, -motor_body_len / 4, h);
    }
    translate([-motor_body_len / 3, 0, 0])
    rotate([0, 0, -90]) {
        triangle(triangle_cut_height, motor_body_len / 4, h);
        triangle(triangle_cut_height, -motor_body_len / 4, h);
    }
    translate([motor_body_len / 3, 0, 0])
    rotate([0, 0, 90]) {
        triangle(triangle_cut_height, motor_body_len / 4, h);
        triangle(triangle_cut_height, -motor_body_len / 4, h);
    }
}

module fc_pcb() {
    fc_width = 20;
    fc_height = 28;
    if(with_pcbs)
        cube([fc_width, fc_height, 2]);
}

module rx_pcb() {
    rx_width = 12;
    rx_height = 25;
    if(with_pcbs)
        cube([rx_width, rx_height, 2]);
}

module motor() {
    color([0.8,0.2,0.2,0.5]) {
        if (with_motor) {
            cylinder(d = motor_size, h = motor_height );
            translate([0, 0, -5])
                cylinder(d = 2, h = motor_height + 5);
        }
        if (with_props) {
            translate([0, 0, -5])
                cylinder(d = 66.5, h = 2);
        }
    }
}

module motor_clamp() {
    // motor / prop mockup

    difference() {
        union () {
            color("blue")
            // motor stand
            hull() {
                cylinder(d = motor_size + motor_wall_d, h = motor_height);

                translate([-motor_arm_width / 2, -(motor_size + motor_wall_d) / 2 - 2, motor_height - motor_arm_height])
                    cube([motor_arm_width, 1, motor_arm_height]);
            }

            // print support
            color("green")
            translate([0, 0, motor_height - motor_helper_height])
                cylinder(d = motor_helper_disc, h = motor_helper_height);

            color("blue")

            translate([0, 0, -2])
                cylinder(d = motor_size + motor_wall_d,h=7);
        }

        // cable hole
        translate([-cable_cut_width / 2, -(motor_size + motor_wall_d) / 2 - cable_cut_height, 0])
            cube([cable_cut_width, cable_cut_height, 25]);

        translate([0, -(motor_size) / 2 , motor_height- cable_cut_height])
            aligned_cube([cable_cut_width, cable_cut_height, cable_cut_height]);

        // motor hole
        translate([0, 0, -1])
            cylinder(d = motor_size, h = motor_height + 2);

        // clamp cutout
        translate([-motor_clamp_cut / 2, -3, -motor_size - motor_wall_d - 1])
            cube([motor_clamp_cut, motor_size + motor_wall_d + 5, motor_height + motor_size + motor_wall_d + 2]);
        translate([0, 0, -motor_size - motor_wall_d - 1])
            cylinder(d=6.5, h= motor_height + motor_size + motor_wall_d + 2);
        translate([0, 0, -1.75])
            cylinder(d2=motor_size,d1=6.5, h=0.75);

        //sphere(d = motor_size);
    }

    %motor();
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

            translate([-cable_cut_width / 2, -motor_arm_length - ((motor_size + motor_wall_d) / 2) - 2, motor_height - motor_arm_height-3])

                cube([cable_cut_width, motor_arm_length + 2, cable_cut_height + 3]);
        }
    }

    color("red")
    translate([0,0, motor_height])
    difference() {
        aligned_cube([motor_body_len, motor_body_len, motor_body_height],[1,1,2]);

        for(r = [45 : 90 : 360]) {
            translate([0,0, -motor_height + motor_body_height])
                rotate([0, 0, r])
                translate([0, motor_body_len / 2 * sqrt(2) - 2, cable_cut_height]) {
                    aligned_cube([cable_cut_width, 8, cable_cut_height ]);
                }
        }
    }
}

module quad() {
    difference() {
        translate([0, 0, motor_height])
        rotate([180, 0, 0])
        {
            body();

            translate([0,0, 15])
            {
                translate([-3, -14, 0])
                    %fc_pcb();

                translate([-16, -12.5, 0])
                    %rx_pcb();
            }
        }

        // cut out for actual PCBs
        translate([-(motor_body_len - (2 * motor_body_wall)) / 2, -(motor_body_len - (2 * motor_body_wall)) / 2, motor_body_wall])
            cube([motor_body_len - (2 * motor_body_wall), motor_body_len - (2 * motor_body_wall), motor_body_height - motor_body_wall + 1]);

        // akku cutout
        akku_cut=motor_body_len - motor_body_wall*2;
        translate([0,0, motor_body_height]) {
            scale([1,1,1])
            rotate([0,0,45])
            //cylinder($fn=4, d1=akku_cut*1.4, d2=30,  h=11);
            translate([1,-12.5,0]) {
                //cube([20,25,10]);
            }
            translate([-30,-12.5,0]) {
                //cube([20,25,10]);
            }
        }
        translate([-motor_body_len/2,-(akku_cut)/2, motor_body_height]) {
            //cube([motor_body_len, akku_cut/2, 10]);
        }

        // usb cutout
        translate([-2, -motor_body_len / 2 - 1, 2])
            cube([usb_width, 4, usb_hight]);

        // triangle cut-outs
        translate([0, 0, -1])
            triangle_cuts();
    }
}
module akku_holder() {
    color([0.5,0.8,0.4,0.5])
    difference() {
        union() {
            aligned_cube([motor_body_len,motor_body_len,akku_holder_hight]);
            translate([0,0,-1])
                aligned_cube([motor_body_len-motor_body_wall*2,
                              motor_body_len-motor_body_wall*2,
                              akku_holder_hight]);
        }
        translate([0,0,0.01]) {
            aligned_cube([akku_holder_width,
                motor_body_len+1,
                akku_holder_hight+2]);
        }
        translate([0, 0, -2])
            triangle_cuts(h=6);
    }
}

quad();

translate([0,0,motor_body_height]) {
    akku_holder();
}
