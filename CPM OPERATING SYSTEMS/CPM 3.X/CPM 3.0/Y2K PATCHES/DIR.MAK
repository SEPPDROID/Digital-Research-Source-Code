:F1:plm80 main80.plm debug pagewidth(130) optimize object(main80) 
:F1:plm80 scan.plm debug pagewidth(130) optimize object(scan)
:F1:plm80 search.plm debug pagewidth(130) optimize object(search)
:F1:plm80 sort.plm debug pagewidth(130) optimize object(sort)
:F1:plm80 disp.plm debug pagewidth(130) optimize object(disp) 
:F1:plm80 dpb80.plm debug pagewidth(130) optimize object(dpb80)
:F1:plm80 util.plm debug pagewidth(130) optimize object(util)
:F1:plm80 timest.plm debug pagewidth(130) optimize object(timest)
:F3:link mcd80a.obj,main80,scan,search,sort,disp,util,dpb80,timest,:F1:plm80.lib to dir.mod
:F3:locate dir.mod code(0100H) stacksize(50)
:F3:objhex dir to dir.hex
exit
