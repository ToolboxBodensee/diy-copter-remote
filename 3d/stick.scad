// hight of stick from the cone
// throttle has different mounting slot at the bottom then the roll stick
// throttle has a metal stick and roll has a ps2 plastic shaft
use <lib/cube.scad>
use <lib/cylinder.scad>

is_ps2_stick=0;
// draw itself
stick(10, is_ps2_stick);

module stick(h=10,is_ps2_stick=0)
{
    $fn=128;
    translate([0,0,15-3]) {
        cylinder(d=5,h=h);
        translate([0,0,h])
            // top nob to grab
            gabber_part();
    }
    // stick holder with cutout
    translate([0,0,0]) {
        stick_mount(is_ps2_stick);
    }

    // dust protector
    translate([0,0,0]) {
        resize([30,30,22])
        protector_cone(thickness=0.4*2.5);
    }
}

module gabber_part() {
    difference() {
        union() {
            cylinder(d1=5,d2=8,h=4);
            translate([0,0,4])
                cylinder(d1=8,d2=8,h=4);
            translate([0,0,8])
                cylinder(d1=8,d2=4,h=3);
        }
        for (rot=[0:15:360]) {
            r=0.5;
            // top grips
            translate([0,0,8])
            rotate([0,0,rot])
            rotate([0,90,0])
            translate([-3,0,0])
                cylinder($fn=4,r=r,h=40);

            // head grips
            translate([0,0,8.5])
            rotate([0,0,rot])
            rotate([0,35,0])
            translate([-3,0,-10])
                cylinder($fn=4,r=r,h=40);

            // vertiacal grips
            rotate([0,0,rot])
            translate([4,0,0])
                cylinder($fn=4,r=r,h=10);
            // bottom grips
            translate([0,0,2])
            rotate([0,0,rot])
            rotate([0,21,0])
            translate([3,0,0])
                cylinder($fn=4,r=r,h=20);
        }
    }
}

module stick_mount(is_ps2_stick=0) {
    ps2_dia=4+0.4;
    ps2_width=3+0.4;


    mount_h=5;
    eps = 0.01;

    difference() {
        color([1,0,1]) {
            translate([0,0,4])
                cylinder_flange_sphere($fn=32,r2=8, r1=3, h=4.5);
            translate([0,0,8.5])
                cylinder_flange_sphere($fn=32,r2=3, r1=8, h=4.5);

            if ( is_ps2_stick  ) {
                cylinder(d=6.75,h=10);
            } else {
                cylinder(d=6,h=10);
            }
            //cylinder(d=15,h=0.1);
        }
        translate([0,0,-eps])
        if ( is_ps2_stick) {
            intersection() {
                cube(center=true,[ps2_width,100,100]);
                cylinder(d=ps2_dia,h=mount_h);
            }
            translate([0,0,mount_h])
                cylinder(d1=ps2_dia,d2=1.5,h=1.5);
        } else {
            aligned_cube([1.9,1.15,6]);
        }
    }
}
module protector_cone(thickness) {
    d=30;
    //dust protector
    difference() {
        union() {
            sphere(d=d+2*thickness);
            translate([0,0,12.1])
            //cylinder(d2=3,d1=17.5,h=7);
            cylinder_flange_sphere($fn=32,r2=4/2, r1=17.5/2, h=20);
        }

        // cut lower half
        translate([-d,-d,-30])
            cube([2*d,2*d,30]);

        // cut innerpart so its a shell
        sphere(d=d);
    }
}



