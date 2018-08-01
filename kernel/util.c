
#include "util.h"

void kmemory_copy(char* src, char* dest, int nbytes) {
    int i;
    for (i = 0;i < nbytes;i++) {
	*(dest + i) = *(src + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    while (len) {
	*temp++ = val;
	len --;
    }
}

// K&R
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

// K&R
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = string_len(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int string_len(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}
