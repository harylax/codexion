*This project has been created as part of the 42 curriculum by haryandr*

# LIBFT

## Description

Libft is the first project of the 42's curriculum. It is a C library that reimplements useful fuctions from the libc, and also creates additional fuctions.
The goal of the project is to build a C library that can be used for further projects, and improve basic C programming skills while building it.
The libft functions concerns:
- Character check: `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, etc.
- Memory operation: `ft_memset`, `ft_memcpy`, `ft_memmove`, etc.
- Memory management: `ft_calloc`.
- Character manipulation: `ft_toupper`, `ft_tolower`.
- Conversion: `ft_atoi`, `ft_itoa`.
- String manipulation: `ft_strlen`, `ft_strlcat`, `ft_split`, etc.
- Displaying in file: `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`, etc.
- Linked list: `ft_lstadd_back`, `ft_lstclear`, `ft_lstsize`, etc. 

## Instructions

This project produces a static C library and does not require installation.
It is meant to be compiled and linked directly with other C projects.

To compile the library using:
```bash
make
```
This will generate `libft.a`

To use it in another project:
```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   your_program_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_email_adress>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 06:57:15 by your_login        #+#    #+#             */
/*   Updated: 2026/01/26 06:21:29 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

int	main(void)
{
	char	str[] = "hello world";
	int		fd;

	fd = open("your_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_putendl_fd(str, fd);
	close(fd);
	return (0);
}

```
To compile your program with libft.a:
```bash
cc your_program_name.c libft.a
```

Available makefile command:
```bash
make		#build the library
```
```bash
make clean	#remove object files
```
```bash
make fclean	#remove the library and object files
```
```bash
make re		#cleans and rebuild the library
```
## Resources

- C man pages
- 42 libft subject
- youtube tutorials about linked list

### AI usage

AI was not used for this project. Everything was implemented, reviewed and tested manually.

## Implemented functions


### Libc functions reimplemented

| Function     | Prototype                                                                 | Description                                                      |
| ------------ | ------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| `ft_isalpha` | `int ft_isalpha(int c);`                                                  | Checks if the character is an alphabetical letter (A–Z or a–z).  |
| `ft_isdigit` | `int ft_isdigit(int c);`                                                  | Checks if the character is a digit (0–9).                        |
| `ft_isalnum` | `int ft_isalnum(int c);`                                                  | Checks if the character is alphanumeric.                         |
| `ft_isascii` | `int ft_isascii(int c);`                                                  | Checks if the character belongs to the ASCII character set.      |
| `ft_isprint` | `int ft_isprint(int c);`                                                  | Checks if the character is printable, including space.           |
| `ft_strlen`  | `size_t ft_strlen(const char *s);`                                        | Returns the length of the string, excluding the null terminator. |
| `ft_memset`  | `void *ft_memset(void *s, int c, size_t n);`                              | Fills a block of memory with a specified value.                  |
| `ft_bzero`   | `void ft_bzero(void *s, size_t n);`                                       | Sets a block of memory to zero.                                  |
| `ft_memcpy`  | `void *ft_memcpy(void *dest, const void *src, size_t n);`                 | Copies memory from source to destination (no overlap handling).  |
| `ft_memmove` | `void *ft_memmove(void *dest, const void *src, size_t n);`                | Copies memory safely, handling overlapping memory areas.         |
| `ft_strlcpy` | `size_t ft_strlcpy(char *dst, const char *src, size_t size);`             | Copies a string with size limitation and null-termination.       |
| `ft_strlcat` | `size_t ft_strlcat(char *dst, const char *src, size_t size);`             | Appends a string with size limitation.                           |
| `ft_toupper` | `int ft_toupper(int c);`                                                  | Converts a lowercase letter to uppercase.                        |
| `ft_tolower` | `int ft_tolower(int c);`                                                  | Converts an uppercase letter to lowercase.                       |
| `ft_strchr`  | `char *ft_strchr(const char *s, int c);`                                  | Locates the first occurrence of a character in a string.         |
| `ft_strrchr` | `char *ft_strrchr(const char *s, int c);`                                 | Locates the last occurrence of a character in a string.          |
| `ft_strncmp` | `int ft_strncmp(const char *s1, const char *s2, size_t n);`               | Compares two strings up to a specified number of characters.     |
| `ft_memchr`  | `void *ft_memchr(const void *s, int c, size_t n);`                        | Searches for a byte in a block of memory.                        |
| `ft_memcmp`  | `int ft_memcmp(const void *s1, const void *s2, size_t n);`                | Compares two blocks of memory.                                   |
| `ft_strnstr` | `char *ft_strnstr(const char *big, const char *little, size_t len);`      | Locates a substring within a limited length.                     |
| `ft_atoi`    | `int ft_atoi(const char *nptr);`                                          | Converts a string to an integer.                                 |
| `ft_calloc`  | `void *ft_calloc(size_t nmemb, size_t size);`                             | Allocates memory and initializes it to zero.                     |
| `ft_strdup`  | `char *ft_strdup(const char *s);`                                         | Duplicates a string by allocating memory.                        |

### Additional functions

| Function        | Prototype                                                         | Description                                                        |
| --------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------ |
| `ft_substr`     | `char *ft_substr(char const *s, unsigned int start, size_t len);` | Extracts a substring from a string.                                |
| `ft_strjoin`    | `char *ft_strjoin(char const *s1, char const *s2);`               | Concatenates two strings into a new string.                        |
| `ft_strtrim`    | `char *ft_strtrim(char const *s1, char const *set);`              | Trims specified characters from the beginning and end of a string. |
| `ft_split`      | `char **ft_split(char const *s, char c);`                         | Splits a string into an array of strings using a delimiter.        |
| `ft_itoa`       | `char *ft_itoa(int n);`                                           | Converts an integer to a string.                                   |
| `ft_strmapi`    | `char *ft_strmapi(char const *s, char (*f)(unsigned int, char));` | Applies a function to each character of a string.                  |
| `ft_striteri`   | `void ft_striteri(char *s, void (*f)(unsigned int, char*));`      | Applies a function to each character of a string by reference.     |
| `ft_putchar_fd` | `void ft_putchar_fd(char c, int fd);`                             | Writes a character to a file descriptor.                           |
| `ft_putstr_fd`  | `void ft_putstr_fd(char *s, int fd);`                             | Writes a string to a file descriptor.                              |
| `ft_putendl_fd` | `void ft_putendl_fd(char *s, int fd);`                            | Writes a string followed by a newline to a file descriptor.        |
| `ft_putnbr_fd`  | `void ft_putnbr_fd(int n, int fd);`                               | Writes an integer to a file descriptor.                            |

### Linked list functions

| Function          | Prototype                                                                  | Description                                                |
| ----------------- | -------------------------------------------------------------------------- | ---------------------------------------------------------- |
| `ft_lstnew`       | `t_list *ft_lstnew(void *content);`                                        | Creates a new list element.                                |
| `ft_lstadd_front` | `void ft_lstadd_front(t_list **lst, t_list *new);`                         | Adds an element at the beginning of the list.              |
| `ft_lstsize`      | `int ft_lstsize(t_list *lst);`                                             | Returns the number of elements in the list.                |
| `ft_lstlast`      | `t_list *ft_lstlast(t_list *lst);`                                         | Returns the last element of the list.                      |
| `ft_lstadd_back`  | `void ft_lstadd_back(t_list **lst, t_list *new);`                          | Adds an element at the end of the list.                    |
| `ft_lstdelone`    | `void ft_lstdelone(t_list *lst, void (*del)(void*));`                      | Deletes a single element using a delete function.          |
| `ft_lstclear`     | `void ft_lstclear(t_list **lst, void (*del)(void*));`                      | Deletes and frees all elements of a list.                  |
| `ft_lstiter`      | `void ft_lstiter(t_list *lst, void (*f)(void *));`                         | Applies a function to each element of the list.            |
| `ft_lstmap`       | `t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));` | Creates a new list by applying a function to each element. |