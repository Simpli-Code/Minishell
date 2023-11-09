/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 20:32:38 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/08 19:33:11 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

__sig_atomic_t	g_state;




/*
	A very simple Prompt function
	The PROMPT is defined in minishell.h
	The buffer cwd can be used to store
	the current working directory
	fd 2, to distinguish between ineractive prompts and
	stdout. it also allows easy redirections, suppress or modify
	from stdout while seeing the prompt.
*/
static void	put_prompt(char *envp[])
{
	char	cwd[4097];
	(void)envp;
	getcwd(cwd, 4096);
	write(2, PROMPT, 27);
}








/*
	A more comprehensive Prompt function

	Extract username and hostname from envp
	Add color and formatting to the prompt

*/
/*
static void	put_prompt(char *envp[])
{
	char	cwd[4097];
	char	*user;
	char	*hostname;
	char	prompt[1024];

	getcwd(cwd, 4096);
	user = get_env(envp, "USER");
	hostname = get_env(envp, "HOSTNAME");
	snprintf(prompt, sizeof(prompt), "\033[1;32m%s@%s\033[0;0m:\033[1;34m%s\033[0;0m$ ", user, hostname, cwd);
	write(2, prompt, strlen(prompt));
}
*/






/*
	if SIGINT sets the g_var to Ctrl+C meaning 130 to handle "heredoc"
	The system call 'ioctl' is to simulate the input
	of a newline character as if user had pressed Enter.
	To ensure that the current line is not interrupted by Ctrl+C
	rl_on_new_line to indicate that a new line should be started
	TIOCSTI is an integer value to inject a newline into the input stream
	as if it had been typed by user.
*/
static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_state = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
	}
}






/*
	to handle Ctrl+C signals in a user interface, ensuring that the
	terminal behaves as expected when the user presses Ctrl+C.
	It sets an exit status code, prints a newline,
	clears the current input line, and redisplays the prompt
*/
//void	std_sighandler(int signum)
//{
//	if (signum == SIGINT)
//	{
//		g_state = 130;
//		ft_putchar_fd('\n', 1);
//		rl_on_new_line();
//		rl_replace_line("", 0);
//		rl_redisplay();
//	}
//}






/*
	To Initialize and malloc the data structure
	*data	a pointer to t_data
	envp	array of strings containing environment variables.
			it calls copy_env func to create a copy of envp
	export	malloc for an array of strings
	argv	array of strings representing command line arguments
	ret		initialize ret to 0
	str		initialize str to 0 indicating empty str
	child	initialize to 0, to track chile proccesses
	ret_len	array of integers with 2 elem. (return values)
	ret_len[0] = 1 to indicate that initialization is complete

*/
static void	init_param(t_data **data, char **argv, char **envp, int *ret_len)
{
	(*data) = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return ;
	(*data)->envp = copy_env(envp, 0);
	(*data)->export = (char **)ft_calloc(sizeof(char *), 1);
	(*data)->argv = argv;
	(*data)->ret = 0;
	(*data)->str = 0;
	(*data)->child = 0;
	ret_len[0] = 1;
}





/*
	If more than one argument return.
	handling signals SIGQUIT & SIGINT
	inside the infinite loop check repeatedly
	if (ret_len[0] is non-zero) display the prompt
	handle Ctrl+C signal
	read character from stdin until \n is found
	store the readed characters in data-str
	strlen will retrun the len of readed characters
	if \n calls parser function to proccess the input
	if both ret_len[0] and ret_len[1] are zero
		exit the terminal Ctrl+D -> terminate the program
	return 0 to inicate successful execution
*/
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		ret_len[2];
	char	c;

	if (argc != 1)
		return (1);
	init_param(&data, argv, envp, ret_len);
	signal(SIGQUIT, sig_handler);
	while (1)
	{
		if (ret_len[0])
			put_prompt(envp);
		signal(SIGINT, sig_handler);
		while ((ret_len[0] = read(1, &c, 1)) && c != '\n')
			ft_addchr(&(data->str), c);
		ret_len[1] = (int)ft_strlen(data->str);
		if (c == '\n')
			parser(data);
		if (!ret_len[0] && !ret_len[1])
		{
			ft_putstr_fd("\nexit\n", 2);
			exit(0);
		}
		add_history(data->str);
	}
	return (0);
}

/*
 it loops thorough each argv, until it finds a cha"<"
*/
char	**copy_args(t_data *data)
{
	int		i;
	char	**args;

	i = 0;
	while (data->argv[i] && ft_memcmp(data->argv[i], "<", 2))
		i++;
	args = ft_calloc(sizeof(char *), i + 1);
	i = 0;
	while (data->argv[i] && ft_memcmp(data->argv[i], "<", 2))
	{
		args[i] = ft_strdup(data->argv[i]);
		i++;
	}
	return (args);
}

static int	ft_strlen_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '<' || str[i] == '>' || str[i] == '=' || str[i] == '|')
		i = (str[i] == '>' && str[i + 1] == '>') ? 2 : 1;
	else
	{
		while (str[i] && !ft_isspace(str[i]) && str[i] != '<' &&
				str[i] != '>' && str[i] != '=' && str[i] != '|')
		{
			if (str[i] == '\'' || str[i] == '"')
			{
				i++;
				i += ft_strlen_arg_token(str + i, str[i - 1]);
				if (!(str[i]))
					return (i);
			}
			i++;
		}
		if (str[i] == '=')
			i++;
	}
	return (i);
}

int	count_args(char *str)
{
	int	n;

	n = 0;
	ft_skip_spaces(&str);
	while (*str)
	{
		ft_skip_spaces(&str);
		n++;
		str += ft_strlen_arg(str);
		ft_skip_spaces(&str);
	}
	return (n);
}

void	set_args(char **argv, char *str, int argc)
{
	int	i;
	int	len;

	i = 0;
	ft_skip_spaces(&str);
	while (i < argc)
	{
		ft_skip_spaces(&str);
		len = ft_strlen_arg(str);
		argv[i] = ft_strldup(str, len);
		rm_token(&(argv[i]));
		i++;
		str += len;
		ft_skip_spaces(&str);
	}
}

static int	check_errno(t_data *data, char *str)
{
	if (errno == ENOENT || errno == EACCES)
	{
		ft_putstrs_fd("bash: ", str, ": ", 2);
		ft_putstrs_fd(strerror(errno), "\n", 0, 2);
		data->ret = (errno == ENOENT) ? 127 : 126;
		return (1);
	}
	return (0);
}

static void	check_type(t_data *data, char *str, char *path)
{
	DIR		*dir;
	int		fd;
	char	**cmds;

	if (check_errno(data, str))
		return ;
	else if (!(dir = opendir(path)))
	{
		fd = open(path, O_RDONLY, 0666);
		free(data->str);
		while (get_next_line(fd, &(data->str)))
		{
			cmds = data->cmds;
			parser(data);
			data->cmds = cmds;
		}
		close(fd);
	}
	else
	{
		ft_putstrs_fd("-bash: ", str, ": Is a directory\n", 2);
		data->ret = 126;
		closedir(dir);
	}
}

static void	set_filename(int len, char **new, char *str)
{
	int		i;
	char	*filename;
	char	*aux;

	i = 0;
	while (!ft_memcmp(str + i, "../", 3))
		i += 3;
	filename = ft_strdup(str + i);
	i /= 3;
	while (i-- > 0)
	{
		while ((*new)[len] != '/')
			len--;
		len--;
	}
	aux = ft_strldup(*new, len);
	free(*new);
	*new = ft_strjoin(aux, "/");
	free(aux);
	aux = ft_strjoin(*new, filename);
	free(*new);
	*new = aux;
	free(filename);
}

static void	set_path(char *str, char **path)
{
	int		len;
	char	*new;

	new = ft_strdup(*path);
	len = ft_strlen(*path);
	if (!ft_memcmp(str, "/", 1))
		*path = ft_strdup(str);
	else
	{
		set_filename(len, &new, str);
		*path = new;
		return ;
	}
	free(new);
}

void	bash_command(t_data *data)
{
	char	buff[4097];
	char	*path;
	char	*start;

	start = data->argv[0];
	if (ft_memcmp(data->argv[0], "/", 1))
		data->argv[0] += (!ft_memcmp(data->argv[0], "./", 2)) ? 2 : 0;
	path = getcwd(buff, 4096);
	set_path(data->argv[0], &path);
	if (!fork())
	{
		signal(SIGINT, child_sig_handler_bash);
		if (execve(path, data->argv, data->envp))
			check_type(data, start, path);
		exit(data->ret);
	}
	else
		wait(&data->ret);
	data->ret /= 256;
	free(path);
	data->argv[0] = start;
}

static char	**multiple_env(t_data *data, int fd)
{
	int	i;

	data->ret = 0;
	if (!ft_memcmp(data->argv[0], "export", 7) && data->argc == 1 &&
		!ft_strlen(data->argv[1]))
	{
		sort_envp(data->envp, fd, '=');
		sort_envp(data->export, fd, 0);
	}
	i = 1;
	while (data->argv[i])
	{
		if (check_export_error(data->argv, &i))
			(data->ret)++;
		else
		{
			if (!ft_memcmp(data->argv[0], "export", 7))
				export_value(data, &i);
			else if (!ft_memcmp(data->argv[0], "unset", 6))
				data->envp = unset_command(data, i++);
		}
	}
	data->ret = data->ret ? 1 : 0;
	return (data->envp);
}

static void	env_command(t_data *data, int fd)
{
	int	i;

	i = 0;
	if (data->argc != 1)
	{
		ft_putstrs_fd("env: ‘", data->argv[1], "’: Permission denied\n", 2);
		data->ret = 126;
		return ;
	}
	while (data->envp[i])
		ft_putstrs_fd(data->envp[i++], "\n", 0, fd);
}

static void	echo_command(t_data *data, int fd)
{
	int	i;

	i = (data->argc > 1 && !ft_memcmp(data->argv[1], "-n", 3)) ? 1 : 0;
	while (++i < data->argc)
	{
		ft_putstr_fd(data->argv[i], fd);
		if (i < data->argc - 1 && ft_strlen(data->argv[i + 1]))
			write(fd, " ", 1);
	}
	if (!(data->argc > 1 && !ft_memcmp(data->argv[1], "-n", 3)))
		write(fd, "\n", 1);
}

static int	check_builts(int fd, t_data *data)
{
	char	*path;
	char	cwd[4097];

	path = 0;
	if (!ft_memcmp(data->argv[0], "echo", 5))
		echo_command(data, fd);
	else if (!ft_memcmp(data->argv[0], "pwd", 4))
		ft_putstrs_fd(getcwd(cwd, 4096), "\n", 0, fd);
	else if (!ft_memcmp(data->argv[0], "cd", 3))
	{
		path = ft_strdup(data->argv[1]);
		cd_command(data);
		if (path && !ft_strncmp(path, "-", 2))
			ft_putstrs_fd(getcwd(cwd, 4096), "\n", 0, fd);
		free(path);
	}
	else
		return (1);
	return (0);
}

int	check_builtins(int fd, t_data *data)
{
	data->ret = 0;
	if (!check_builts(fd, data))
		return (data->ret);
	else if (!ft_memcmp(data->argv[0], "env", 4))
		env_command(data, fd);
	else if (!ft_memcmp(data->argv[0], "./", 2) ||
				!ft_memcmp(data->argv[0], "../", 3) ||
				!ft_memcmp(data->argv[0], "/", 1))
		bash_command(data);
	else if (!ft_memcmp(data->argv[0], "export", 7) ||
				!ft_memcmp(data->argv[0], "unset", 6))
		data->envp = multiple_env(data, fd);
	else if (!ft_memcmp(data->argv[0], "exit", 5) ||
				!ft_memcmp(data->argv[0], "q", 2))
		exit_command(data);
	else
		return (127);
	return (data->ret);
}

static void	change_dir(char *path, t_data *data)
{
	char	cwd[4097];
	char	oldpwd[4097];

	getcwd(oldpwd, 4096);
	if (chdir(path) == 0)
	{
		data->argc = 4;
		free_matrix(data->argv);
		data->argv = (char **)ft_calloc(sizeof(char *), 4);
		data->argv[0] = ft_strdup("export");
		data->argv[1] = ft_strdup("OLDPWD=");
		data->argv[2] = ft_strdup(oldpwd);
		data->envp = export_command(data, 1);
		free_matrix(data->argv);
		data->argv = (char **)ft_calloc(sizeof(char *), 4);
		data->argv[0] = ft_strdup("export");
		data->argv[1] = ft_strdup("PWD=");
		data->argv[2] = ft_strdup(getcwd(cwd, 4096));
		data->envp = export_command(data, 1);
	}
	else
		ft_putstrs_fd("bash: cd: ", data->argv[1], ": ", 2);
}

void	cd_command(t_data *data)
{
	char	*path;

	errno = 0;
	if (data->argc <= 2)
	{
		if (!data->argv[1] || !ft_strncmp(data->argv[1], "--", 3) ||
			!ft_strncmp(data->argv[1], "~", 2))
			path = get_env(data->envp, "HOME");
		else if (!ft_strncmp(data->argv[1], "-", 2))
			path = get_env(data->envp, "OLDPWD");
		else
			path = data->argv[1];
		change_dir(path, data);
		if (errno > 0)
		{
			ft_putstrs_fd(strerror(errno), "\n", 0, 2);
			data->ret = 1;
		}
	}
	else
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		data->ret = 1;
	}
	errno = 0;
}

static void	set_in(char **argv)
{
	int	fd;
	int	i;

	i = 0;
	while (argv[i] && ft_memcmp(argv[i], "<", 2))
		i++;
	if (argv[i])
	{
		fd = open(argv[i + 1], O_RDONLY, 0666);
		if (fd < 0)
		{
			ft_putstr_fd("Couldn't read from file.\n", 2);
			return ;
		}
		dup2(fd, 0);
		close(fd);
	}
}

static void	exec_bin(int fd, char *path, t_data *data)
{
	char	**args;

	args = copy_args(data);
	signal(SIGINT, child_sig_handler);
	if (!fork())
	{
		set_in(data->argv);
		if (fd > 1)
			dup2(fd, 1);
		if ((execve(path, args, data->envp)) && errno == EACCES)
		{
			data->ret = 126;
			ft_putstrs_fd("bash: ", data->argv[0], ": ", 2);
			ft_putstrs_fd(strerror(errno), "\n", 0, 2);
		}
		exit(data->ret);
	}
	wait(&data->ret);
	data->ret /= 256;
	free(path);
	free_matrix(args);
}

static char	**split_path(t_data *data, char *str)
{
	char	*path;
	char	**paths;

	path = get_env(data->envp, "PATH");
	if (path)
		paths = ft_split_case(path, ':');
	else
	{
		ft_putstrs_fd("bash: ", str, ": No such file or directory\n", 2);
		data->ret = 127;
		return (NULL);
	}
	return (paths);
}

static char	*search_bin(char *str, DIR **dir, struct dirent **d, t_data *data)
{
	char	**paths;
	char	*path;
	int		i;

	if (!(paths = split_path(data, str)))
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		*dir = opendir(paths[i]);
		while ((*dir) && errno != EACCES && (*d = readdir(*dir)))
		{
			if (!ft_memcmp(str, (*d)->d_name, ft_strlen(str) + 1))
			{
				path = ft_strjoin(paths[i], "/");
				free_matrix(paths);
				return (path);
			}
		}
		closedir(*dir);
	}
	free_matrix(paths);
	return (NULL);
}

int	check_bin(int fd, t_data *data)
{
	DIR				*dir;
	struct dirent	*d;
	char			*pre_path;
	char			*path;

	data->ret = 127;
	pre_path = search_bin(data->argv[0], &dir, &d, data);
	if (pre_path)
	{
		path = ft_strjoin(pre_path, d->d_name);
		exec_bin(fd, path, data);
		closedir(dir);
	}
	free(pre_path);
	return (data->ret);
}

static int	redirect(t_data *data, int i, int fd)
{
	int		ret;
	char	c;

	while (data->argv[i])
	{
		if (!ft_memcmp(data->argv[i], ">", 2))
			fd = open(data->argv[i + 1], O_RDWR | O_CREAT | O_TRUNC, 0666);
		else if (!ft_memcmp(data->argv[i], ">>", 3))
		{
			fd = open(data->argv[i + 1], O_RDWR | O_CREAT | O_APPEND, 0666);
			ret = 0;
			while ((ret = read(fd, &c, 1)))
				if (ret == -1)
				{
					write(2, "Couldn't read file\n", 19);
					break ;
				}
		}
		i++;
		if (data->argv[i] &&
			(!ft_memcmp(data->argv[i], ">>", 3) ||
				!ft_memcmp(data->argv[i], ">", 2)))
			close(fd);
	}
	return (fd);
}

static int	set_fd(t_data *data)
{
	int	i;
	int	fd;

	i = 0;
	fd = 1;
	while (data->argv[i] && ft_memcmp(data->argv[i], ">", 2)
		&& ft_memcmp(data->argv[i], ">>", 3))
		i++;
	if (!data->argv[i])
		return (1);
	return (redirect(data, i, fd));
}

static int	count_redir(t_data *data)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (++i < data->argc)
	{
		if (!ft_memcmp(data->argv[i], ">", 2) ||
			!ft_memcmp(data->argv[i], ">>", 3))
		{
			count++;
			i++;
		}
	}
	return (count);
}

static void	copy_args1(t_data *data)
{
	int		i;
	int		j;
	char	**args;

	data->argc -= count_redir(data) * 2;
	args = (char **)ft_calloc(sizeof(char *), data->argc + 1);
	i = 0;
	j = 0;
	while (j < data->argc)
	{
		if (!ft_memcmp(data->argv[i], ">", 2) ||
			!ft_memcmp(data->argv[i], ">>", 3))
			i += 2;
		else
			args[j++] = ft_strdup(data->argv[i++]);
	}
	free_matrix(data->argv);
	data->argv = args;
}

char	**check_command(char *str, t_data *data)
{
	int	fd;

	if (data->argv[0] && *(data->argv[0]))
	{
		fd = set_fd(data);
		copy_args1(data);
		data->ret = check_builtins(fd, data);
		if (data->ret == 127 && (data->ret = check_bin(fd, data)) == 127)
		{
			ft_putstrs_fd(0, str, ": command not found.\n", 2);
			data->ret = 127;
		}
		if (fd != 1)
			close(fd);
	}
	return (data->envp);
}

static void	free_param(t_data *data)
{
	free(data->str);
	free_matrix(data->envp);
	free_matrix(data->export);
	free_matrix(data->argv);
	free_matrix(data->cmds);
	free(data);
}

void	exit_command(t_data *data)
{
	int	i;

	if (data->argc > 2)
	{
		ft_putstrs_fd("exit\n", "bash: exit: too many arguments\n", 0, 2);
		data->ret = 1;
	}
	else
	{
		i = 0;
		while (data->argc > 1 && ft_isdigit(data->argv[1][i]))
		{
			i++;
		}
		if (data->argc > 1 && data->argv[1][i])
		{
			ft_putstrs_fd("exit\nbash: exit: ", data->argv[1],
					": numeric argument required\n", 2);
			data->ret = 2;
		}
		i = (data->argc > 1 && data->ret != 2)
			? ft_atoi(data->argv[1])
			: data->ret;
		free_param(data);
		exit(i);
	}
}

char	**export_command(t_data *data, int j)
{
	int		i;
	char	**cpy;

	i = 0;
	while (data->envp[i] && ft_memcmp(data->envp[i], data->argv[j],
			ft_strlen(data->argv[j])))
		i++;
	if (!data->envp[i])
	{
		cpy = copy_env(data->envp, 1);
		cpy[i] = ft_strjoin(data->argv[j], data->argv[j + 1]);
		free_matrix(data->envp);
	}
	else
	{
		cpy = data->envp;
		free(data->envp[i]);
		data->envp[i] = ft_strjoin(data->argv[j], data->argv[j + 1]);
	}
	return (cpy);
}

void	export_value(t_data *data, int *i)
{
	char	**aux;
	int		j;

	if (!ft_strchr(data->argv[*i], '='))
	{
		j = 0;
		while (data->export[j] && ft_memcmp(data->export[j], data -> argv[*i],
				ft_strlen(data->argv[*i])))
			j++;
		if (!data->export[j])
		{
			aux = copy_env(data->export, 1);
			aux[j] = ft_strdup(data->argv[*i]);
			aux[j + 1] = 0;
			free_matrix(data->export);
			data->export = aux;
		}
		(*i)++;
	}
	else
	{
		data->envp = export_command(data, *i);
		*i += data->argv[*i + 1] ? 2 : 1;
	}
}

int	check_export_error(char **argv, int *i)
{
	int	j;

	j = 0;
	while (ft_isalnum(argv[*i][j]) || argv[*i][j] == '_' ||
			argv[*i][j] == '#' || argv[*i][j] == '=' || argv[*i][j] == '$')
		j++;
	if (ft_isdigit(argv[*i][0]) || argv[*i][j])
	{
		if (ft_strchr(argv[*i], '='))
		{
			ft_putstrs_fd("bash: ", argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], argv[(*i) + 1],
					"': not a valid identifier\n", 2);
			(*i)++;
		}
		else
		{
			ft_putstrs_fd("bash: ", argv[0], ": `", 2);
			ft_putstrs_fd(argv[*i], "': not a valid identifier\n", 0, 2);
		}
		(*i)++;
		return (1);
	}
	return (0);
}

static void	put_envp(char **aux, int fd)
{
	int	i;

	i = -1;
	while (aux[++i])
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstrlen_fd(aux[i], ft_strlen_char(aux[i], '=') + 1, fd);
		if (ft_strchr(aux[i], '='))
			ft_putstrs_fd("\"", ft_strchr(aux[i], '=') + 1, "\"", fd);
		write(fd, "\n", 1);
	}
	free_matrix(aux);
}

void	sort_envp(char **envp, int fd, char c)
{
	int		i;
	int		len;
	char	**aux;
	char	*tmp;

	aux = copy_env(envp, 0);
	i = 0;
	while (aux[i] && aux[i + 1])
	{
		len = (ft_strlen_char(aux[i], c) > ft_strlen_char(aux[i + 1], c))
			? ft_strlen_char(aux[i], c)
			: ft_strlen_char(aux[i + 1], c);
		if (ft_memcmp(aux[i], aux[i + 1], len) > 0)
		{
			tmp = aux[i];
			aux[i] = aux[i + 1];
			aux[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
	put_envp(aux, fd);
}

int	ft_strlen_pipe(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i] && str[i] != '|')
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i++];
			while (str[i] && str[i] != c)
			{
				if (str[i] == '\\' && is_token(str[i + 1]))
					i++;
				i++;
			}
			if (!str[i])
			{
				ft_putstr_fd("Non finished quotes\n", 2);
				return (i);
			}
		}
		else if (str[i] == '\\')
			i++;
	}
	return (i);
}

int	ft_strlen_arg_token(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
	{
		if (str[i] == '\\' && is_token(str[i + 1]))
			i++;
		i++;
	}
	return (i);
}

int	ft_strlen_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int	ft_strlen_token(char *str)
{
	int	len;

	len = 0;
	while (*str && *str != ' ' && *str != ';' && *str != '|' && *str != '<'
		&& *str != '>' && *str != '"' && *str != '\'' && *str != '=')
	{
		len++;
		str++;
	}
	if (*str == '=')
		len++;
	return (len);
}

int	ft_strlen_env(char *str)
{
	int	len;

	len = 0;
	while (*str &&
			(ft_isalnum(*str) || *str == '{' || *str == '?' || *str == '_'))
	{
		len++;
		str++;
	}
	if (*str == '=')
		len++;
	return (len);
}

static int	change_env(int i, int braces, char **str, t_data *data)
{
	int		len;
	char	*bef;
	char	*aft;
	char	*env;
	char	*aux;

	braces = ((*str)[i + 1] == '{') ? 1 : 0;
	len = (ft_strlen_char(*str + i + 1, ':') < ft_strlen_env(*str + i
				+ 1)) ? ft_strlen_char(*str + i + 1, ':')
		+ 1 : ft_strlen_env(*str + i + 1) + 1 + braces;
	bef = ft_strldup(*str, i);
	aux = ft_strldup(*str + i + 1 + braces, len - 1 - braces * 2);
	env = (!ft_memcmp(aux, "?", 2)) ? ft_itoa(data->ret) : 0;
	aft = ft_strdup(*str + i + len);
	env = (!env) ? ft_strdup(get_env(data->envp, aux)) : env;
	free(aux);
	len = ft_strlen(env);
	aux = ft_strjoin(bef, env);
	free(bef);
	free(env);
	free(*str);
	*str = ft_strjoin(aux, aft);
	free(aux);
	free(aft);
	return (len);
}

static int	check_quotes(char **str, int *i)
{
	(*i)++;
	while ((*str)[*i] && ((*str)[*i] != '\''))
		(*i)++;
	if (!(*str)[*i])
	{
		ft_putstr_fd("Non finished quotes\n", 2);
		return (1);
	}
	return (0);
}

static int	check_env(char **str, t_data *data)
{
	int	i;
	int	braces;

	i = 0;
	braces = 0;
	while ((*str) && (*str)[i])
	{
		if ((*str)[i] == '\'' && check_quotes(str, &i))
			return (1);
		if ((*str)[i] && (*str)[i] == '\\')
		{
			if ((*str)[i + 1] == '$')
				rm_char(str, i);
			if ((*str)[i + 1])
				i++;
		}
		else if ((*str)[i] == '$' && !(!(*str)[i + 1] || ft_isspace((*str)[i
						+ 1]) || (*str)[i + 1] == '\'' || (*str)[i + 1] == '"'
					|| (*str)[i + 1] == '/'))
			i += change_env(i, braces, str, data) - 1;
		i++;
	}
	return (0);
}

static int	check_semicolon(t_data *data)
{
	if (!data->str || !ft_memcmp(data->str, ";", 2))
	{
		if (data->str)
		{
			ft_putstr_fd("-bash; syntax error near unexpected token `;'\n", 2);
			data->ret = 2;
		}
		free(data->str);
		data->str = 0;
		return (1);
	}
	return (0);
}

void	parser(t_data *data)
{
	int	i;

	if (check_semicolon(data))
		return ;
	data->cmds = ft_split_case(data->str, ';');
	i = 0;
	while (data->cmds[i])
	{
		check_env(&(data->cmds[i]), data);
		data->argc = count_args(data->cmds[i]);
		data->argv = (char **)ft_calloc(sizeof(char *), (data->argc + 1));
		set_args(data->argv, data->cmds[i], data->argc);
		command_or_pipe(data, i);
		i++;
		free_matrix(data->argv);
	}
	free(data->str);
	data->str = 0;
	free_matrix(data->cmds);
}

static void	set_pipe_args(t_data *data, int i)
{
	char	**aux;
	int		j;
	int		k;

	j = 0;
	while (data->argv[i + j] && ft_memcmp(data->argv[i + j], "|", 2))
		j++;
	aux = (char **)ft_calloc(sizeof(char *), j + 1);
	k = -1;
	while (++k < j)
		aux[k] = ft_strdup(data->argv[i + k]);
	data->argv = aux;
	data->argc = j;
}

static void	pipe_son(int *flag, int *fds, t_data *data, int pos)
{
	int		i;
	int		argc;
	char	**argv;

	signal(SIGINT, child_sig_handler);
	if (!fork())
	{
		if (!flag[0])
			dup2(fds[0], 0);
		if (!flag[1])
			dup2(fds[3], 1);
		i = 0;
		while (i < 4)
			close(fds[i++]);
		argc = data->argc;
		argv = data->argv;
		set_pipe_args(data, pos);
		check_command(data->str, data);
		free_matrix(data->argv);
		data->argc = argc;
		data->argv = argv;
		exit(data->ret);
	}
}

static void	switch_pipes(int *fds)
{
	close(fds[0]);
	close(fds[1]);
	fds[0] = fds[2];
	fds[1] = fds[3];
	pipe(fds + 2);
}

static int	check_pipe(int *fds, t_data *data)
{
	int	sons;
	int	*flag;
	int	i;
	int	j;

	sons = 0;
	flag = (int *)malloc(sizeof(int) * 2);
	flag[0] = 1;
	flag[1] = 0;
	j = 0;
	while (data->argv[j])
	{
		i = 0;
		while (data->argv[j + i] &&
				ft_memcmp(data->argv[j + i], "|", 2))
			i++;
		flag[1] = (!data->argv[i + j]) ? 1 : 0;
		pipe_son(flag, fds, data, j);
		sons++;
		flag[0] = 0;
		switch_pipes(fds);
		j += !data->argv[j + i] ? i : i + 1;
	}
	free(flag);
	return (sons);
}

void	command_or_pipe(t_data *data, int j)
{
	int	fds[4];
	int	std_out;
	int	sons;
	int	i;

	std_out = dup(0);
	i = 0;
	while (data->argv[i] && ft_memcmp(data->argv[i], "|", 2))
		i++;
	if (!data->argv[i])
		data->envp = check_command(data->cmds[j], data);
	else if (data->cmds[j])
	{
		pipe(fds);
		pipe(fds + 2);
		sons = check_pipe(fds, data);
		while (sons-- > 0)
			wait(&data->ret);
		data->ret /= 256;
		i = -1;
		while (++i < 4)
			close(fds[i]);
	}
	dup2(std_out, 0);
}

void	rm_char(char **str, int j)
{
	char	*bef;
	char	*aux;

	bef = ft_strldup(*str, j);
	aux = ft_strdup(*str + j + 1);
	free(*str);
	*str = ft_strjoin(bef, aux);
	free(aux);
	free(bef);
}

static void	rm_backslash(char **arg, int *i)
{
	while ((*arg)[*i])
	{
		if ((*arg)[*i] == '\\' && is_token((*arg)[*i + 1]))
			rm_char(arg, *i);
		else if ((*arg)[*i] == '"')
			break ;
		(*i)++;
	}
}

void	rm_token(char **arg)
{
	int	i;

	i = 0;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\'')
		{
			rm_char(arg, i);
			i += ft_strlen_char(*arg + i, '\'');
			rm_char(arg, i);
		}
		else if ((*arg)[i] == '"')
		{
			rm_char(arg, i);
			rm_backslash(arg, &i);
			rm_char(arg, i);
		}
		else if (((*arg)[i] == '\\') &&
					(is_token((*arg)[i + 1]) || (*arg)[i + 1] == '\''))
			rm_char(arg, i++);
		else
			i++;
	}
}

static char	**erase_env(char **envp, int i)
{
	int		j;
	int		len;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	if (!(cpy = (char **)ft_calloc(sizeof(char *), len)))
		return (NULL);
	j = -1;
	while (++j < i)
		cpy[j] = ft_strdup(envp[j]);
	i++;
	while (envp[i])
		cpy[j++] = ft_strdup(envp[i++]);
	free_matrix(envp);
	return (cpy);
}

char	**unset_command(t_data *data, int j)
{
	int		i;
	int		len;
	char	*env;
	char	**cpy;

	if (data->argc < 2)
		return (data->envp);
	len = strlen(data->argv[j]);
	env = ft_strjoin(data->argv[j], "=");
	i = 0;
	while (data->envp[i] && ft_memcmp(env, data->envp[i], len + 1))
		i++;
	if (data->envp[i])
		cpy = erase_env(data->envp, i);
	else
		cpy = data->envp;
	free(env);
	return (cpy);
}

void	child_sig_handler(int sig)
{
	if (sig == SIGINT)
		write(2, "\n", 1);
}

void	child_sig_handler_bash(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		exit(0);
	}
}

char	*get_env(char **envp, char *env)
{
	int		i;
	int		len;
	char	*tmp;

	env = ft_strjoin(env, "=");
	len = strlen(env);
	i = 0;
	while (envp[i] && ft_memcmp(env, envp[i], len))
		i++;
	if (envp[i])
		tmp = envp[i] + len;
	else
		tmp = 0;
	free(env);
	return (tmp);
}

char	**copy_env(char **envp, int add)
{
	int		len;
	int		i;
	char	**cpy;

	len = 0;
	while (envp[len])
		len++;
	if (!(cpy = (char **)ft_calloc(sizeof(char *), (len + add + 1))))
		return (0);
	i = -1;
	while (++i < len)
		cpy[i] = ft_strdup(envp[i]);
	return (cpy);
}

int	is_token(char c)
{
	if (c == '"' || c == '\\')
		return (1);
	return (0);
}
