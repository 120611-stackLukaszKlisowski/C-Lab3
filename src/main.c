#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

float add(float a, float b)      { return a + b; }
float subtract(float a, float b) { return a - b; }
float multiply(float a, float b) { return a * b; }
float divide(float a, float b)   { return a / b; }

static int parse_number(const char **ptr, float *val)
{
    if (isspace((unsigned char)**ptr) || **ptr == '\0') return -1;
    errno = 0;
    char *end;
    *val = strtof(*ptr, &end);
    if (end == *ptr) return -1;
    if (errno == ERANGE || !isfinite(*val)) return -2;
    if (*end != ' ') return -1;
    *ptr = end + 1;
    return 0;
}

int main(void)
{
    char buf[128];

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        printf(feof(stdin) ? "error: no input\n" : "error: read failure\n");
        return 0;
    }

    char *nl = strchr(buf, '\n');
    if (!nl && !feof(stdin)) {
        printf("error: input too long\n");
        return 0;
    }
    if (nl) *nl = '\0';

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\r') buf[--len] = '\0';

    const char *ptr = buf;
    float a, b;

    int res_a = parse_number(&ptr, &a);
    if (res_a == -1) { printf("error: invalid input\n"); return 0; }
    if (res_a == -2) { printf("error: invalid number\n"); return 0; }

    int res_b = parse_number(&ptr, &b);
    if (res_b == -1) { printf("error: invalid input\n"); return 0; }
    if (res_b == -2) { printf("error: invalid number\n"); return 0; }

    if (isspace((unsigned char)*ptr) || *ptr == '\0' || ptr[1] != '\0') {
        printf("error: invalid input\n");
        return 0;
    }

    char op = ptr[0];
    float result;

    switch (op) {
        case '+': result = add(a, b); break;
        case '-': result = subtract(a, b); break;
        case '*': result = multiply(a, b); break;
        case '/':
            if (b == 0.0f) {
                printf("error: division by zero\n");
                return 0;
            }
            result = divide(a, b);
            break;
        default:
            printf("error: invalid operator\n");
            return 0;
    }

    if (!isfinite(result)) {
        printf("error: invalid result\n");
        return 0;
    }

    printf("result: %.4f\n", result);
    return 0;
}