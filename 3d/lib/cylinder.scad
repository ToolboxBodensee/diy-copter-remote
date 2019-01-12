module cylinder_flange_sphere($fn=32,r1=10, r2=15, h=5)
{
    diff=abs(r1-r2);
    h_off= (r1 < r2 ) ? 0 : h/2;

    difference() {
        cylinder($fn=$fn, r=max(r2,r1),h=h);
        scale([1,1,h/diff])
        rotate_extrude()
            translate([diff+min(r2,r1),h_off])
                rotate([0,0,0])
                    circle($fn=$fn,r=diff);
    }
}
