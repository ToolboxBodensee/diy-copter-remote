// hight of stick from the cone
// throttle has different mounting slot at the bottom then the roll stick
// throttle has a metal stick and roll has a ps2 plastic shaft
use <lib/cube.scad>
use <lib/cylinder.scad>

// draw itself
stick();

module stick(h=25-10)
{
    $fn=32;
    translate([0,0,15-3]) {
        cylinder(d=5,h=h);
        translate([0,0,h])
            // top nob to grab
            gabber_part();
    }
    // stick holder with cutout
    translate([0,0,0]) {
        stick_mount();
    }

    // dust protector
    translate([0,0,-3]) {
        protector_cone(thickness=1.25);
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

module stick_mount(is_ps2_shaft=1) {
    ps2_dia=4*1.269;
    ps2_hight=5;
    ps2_width=3*1.276;
    eps = 0.01;

    difference() {
        color([1,0,1]) {
            if ( is_ps2_shaft ) {
                cylinder(d=6.75,h=10);
            } else {
                cylinder(d=6,h=10);
            }
        }
        translate([0,0,-eps])
        if ( is_ps2_shaft ) {
            intersection() {
                cube(center=true,[ps2_width,100,100]);
                cylinder(d=ps2_dia,h=ps2_hight);
            }
            translate([0,0,ps2_hight])
                cylinder(d1=ps2_dia,d2=1.5,h=1.5);
        } else {
            aligned_cube([1.9,1.15,6],[0,0,0]);
        }
    }
}
module protector_cone(thickness) {
    //dust protector
    difference() {
        union() {
            sphere(d=30);
            translate([0,0,12.1])
            //cylinder(d2=3,d1=17.5,h=7);
            cylinder_flange_sphere($fn=32,r2=4/2, r1=17.5/2, h=20);
        }

        // cut lower half
        translate([-15,-15,-30])
            cube([30,30,30]);

        // cut innerpart so its a shell
        sphere(d=30-2*thickness);
    }
}



