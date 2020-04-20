
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        mov rax, rdx        ;get row
        mov rdx, 0
        imul rax, rsi       ;row * row width
        add rax, rcx        ;add col - index in rax

        mov rcx, rax        ;save index in rcx

        mov rsi, 8          ;byte offset = index / 8
        idiv rsi            ;byte offset in rax
        add rdi, rax        ;add array base - byte in rdi

        sal rax, 4          ;byte offset * 8
        sub rcx, rax        ;bit offset in rcx
        mov rbx, 7
        sub rbx, rcx        ;7 - bit offset
        mov rcx, rbx
        mov rsi, 1
        sal rsi, cl         ;shift left to create mask

        mov rax, rdi        ;move byte into rax
        or [rax], rsi       ;or byte and mask

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        mov rax, rdx        ;get row
        mov rdx, 0
        imul rax, rsi       ;row * row width
        add rax, rcx        ;add col

        mov rcx, rax        ;index in rcx

        mov rsi, 8          ;computing byte offset
        idiv rsi            ;byte offset in rax
        add rdi, rax        ;add array base address - byte in rdi

        sal rax, 4
        sub rcx, rax        ;bit offset in rcx
        mov rbx, 7
        sub rbx, rcx
        mov rcx, rbx
        mov rsi, 1
        sal rsi, cl         ;create mask in rsi

        mov rax, rdi
        and [rax], rsi

        cmp rax, 0          ;check if bit set
        setg al
        movsx rax, al       ;move result to rax

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
