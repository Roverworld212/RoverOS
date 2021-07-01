target remote localhost:1234
symbol-file ./Sym/rOS.sym
define rr
i r cr0 cr3 cr4 efer rsp rip
end
define vara
i address $arg0
end
define var
p $arg0
end
break bootstrap_start
layout asm
l
set pagination off
continue