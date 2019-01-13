module cylinder_flange_sphere($fn=32,r1=10, r2=15, h=5)
{
    diff=abs(r1-r2);
    h_off= (r1 < r2 ) ? 0 : diff;


    difference()
    {
        cylinder($fn=$fn, r=max(r2,r1),h=h);
        scale([1,1,h/diff])
        rotate_extrude()
            translate([diff+min(r2,r1),h_off])
                rotate([0,0,0])
                    circle($fn=$fn*diff,r=diff);
    }
}
cylinder_flange_sphere($fn=32,r1=4, r2=17.5, h=7);
color([0.5,0.1,0,0.2]) %cylinder(r=17.5,h=20);
translate([0,0,-4]) color([0.0,0.1,0.4,0.2]) %cylinder(r=4,h=25);


translate([40,0])
{
    cylinder_flange_sphere($fn=32,r1=17.5, r2=4, h=7);
    color([0.5,0.1,0,0.2]) %cylinder(r=17.5,h=20);
    translate([0,0,-4]) color([0.0,0.1,0.4,0.2]) %cylinder(r=4,h=25);
}
