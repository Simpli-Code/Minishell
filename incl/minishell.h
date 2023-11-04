
#ifndef _MINISHELL_H_
# define _MINISHELL_H_

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <errno.h>

# define SUCCESS			0
# define FAILURE			1
# define IGNORE				0
# define QUIT				1
# define HERED				2
# define BUF_SIZE			2048
# define TABLE_SIZE			10
# define FNV_prime			0x100000001b3
# define FNV_offset_basis	0xcbf29ce484222325

typedef long long	t_hf(const char *, size_t);




typedef struct s_hash_t
{
	u_int32_t		size;
	u_int32_t		n_elem;
	u_int32_t		n_export;
	u_int32_t		n_val;
	size_t			env_keylen;
	char			*env_str;
	char			*env_value;
	bool			env_has_value;
	bool			env_export;
	struct s_hash_t	*next;
	t_hf			*hash;
}	t_hash_t;


typedef struct s_data
{
	char		*prompt;
	char		**envp;
	char		*buf_str;
	int			buf_size;
	int			buf_cur_pos;
	bool		loop;
	bool		free_prompt;

	t_hash_t	*env_table;
}	t_data;



u_int64_t	fowler_noll_vo(const char *s, size_t len);
t_hash_t	*create_hash(u_int32_t size, t_hf *hf);
int		initialize_buff(t_data *ptr);
void	check_signals(int signum, int code);
void	handle_int(int signum);
void	handle_heredoc(int signum);
void	initialize_data(t_data *data);
void	destroy_hash(t_hash_t *h);




/********UTILS********/

void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
char	*ft_strchr(const char *s, int c);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
void	ft_putendl_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *src);
char	*ft_strjoin(char const *s1, char const *s2);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);


#endif
