/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 20:32:38 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/30 12:09:46 by chruhin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

__sig_atomic_t	g_state;





/*
	To Initialize and malloc the data structure
	*data	a pointer to t_data
	envp	array of strings containing environment variables.
			it calls copy_env func to create a copy of envp
	export	malloc for an array of strings
	argv	array of strings representing cmd line arguments
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
	if SIGINT sets the g_state to Ctrl+C meaning 130 to handle "heredoc"
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
	A very simple Prompt function
	The PROMPT is defined in minishell.h
	The buffer cwd can be used to store
	the current working directory
	fd 2, to distinguish between ineractive prompts and
	stdout. it also allows easy redirections, suppress or modify
	from stdout while seeing the prompt.
*/
//static void	put_prompt(char *envp[])
//{
//	char	cwd[4097];

//	(void)envp;
//	getcwd(cwd, 4096);
//	write(2, PROMPT, 27);
//}

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
	snprintf(prompt, sizeof(prompt),
			"\033[1;32m%s@%s\033[0;0m:\033[1;34m%s\033[0;0m$ ", user, hostname,
			cwd);
	write(2, prompt, strlen(prompt));
}
*/

void	check_signals(int signum, int code)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	errno = 0;
	if (code == 1)
		sa.sa_handler = handle_int;
	else if (code == 2)
		sa.sa_handler = handle_heredoc;
	else if (code == 0)
		sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) != 0)
		perror("signal not found!");
}

void	handle_int(int signum)
{
	if (signum == SIGINT)
	{
		g_state = 130;
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_state = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
	}
}


void	print_history(HIST_ENTRY **hist)
{
	HIST_ENTRY	*item;
	int			i;

	i = 0;
	hist = history_list();
	while (hist[i])
	{
		item = hist[i];
		printf("%d   %s\n", i + 1, item->line);
		i++;
	}
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
int	main(int argc, char *argv[], char *envp[])
{
	t_data	*data;
	int		ret_len[2];
	char	*line;
	//char	c;

	if (argc != 1)
	{
		printf("Too  many arguments: Expected only  ./minishell\n");
		return (1);
	}
	init_param(&data, argv, envp, ret_len);
	signal(SIGQUIT, sig_handler);
	while (1)
	{
		signal(SIGINT, sig_handler);
		if (ret_len[0])
			//put_prompt(envp);
			line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			exit (0);
		}
		ft_addstr(&(data->str), line);
		free (line);
		//while ((ret_len[0] = read(1, &c, 1)) && c != '\n')
			//ft_addchr(&(data->str), c);
		//ret_len[1] = (int)ft_strlen(data->str);
		//if (c == '\n')
			parser(data);
		//if (!ret_len[0] && !ret_len[1])
		//{
			//ft_putstr_fd("\nexit\n", 2);
			//exit(0);
		//}
		ft_strdel(&(data->str));
	}
	//ft_free_b_exit(&line, &data); UNIMPLEMENTED
	exit (g_state);
}


/*
	to create a copy of elements of argv into newly allocated (args)

	it loops thorough each argv, to find the number of elements  until it
	encounters an element that doesn't start with <.
	This number is stored in i.
	allocates memory for a new array of strings args with a size of i + 1
	plus 1 for a NULL pointer to indicate the end of the array.

	loop thorough each element and duplicate
	Free previously allocated memory
	Null-terminate the args array
	returns the args array,
	which is a copy of the relevant elements from data->argv.
*/
char	**copy_args(t_data *data)
{
	int		i;
	int		j;
	char	**args;

	i = 0;
	while (data->argv[i] && ft_memcmp(data->argv[i], "<", 2))
		i++;
	args = ft_calloc(sizeof(char *), i + 1);
	if (!args)
		return (NULL);
	j = 0;
	while (j < i)
	{
		args[j] = ft_strdup(data->argv[j]);
		if (args[j] == NULL)
		{
			free_matrix(args);
			free(args);
			return (NULL);
		}
		j++;
	}
	args[i] = NULL;
	return (args);
}




/*
	calculate the length of a substring within a given string str.
	The length is determined based on certain characters (<, >, =, |),
	spaces, and quoted strings.

	if the character at index i in the input string str is one of the special characters:
	'<', '>', '=', or '|'. If it is, set i to either 1 or 2,
	depending on whether it's a single '>' or a double '>>'.

	If the character at index i is not one of the special characters,
		enter the loop.
	The loop will breaks if it finds a white space or special characters
	if single or dubble quote increament i value (skip over them)
	If the loop encounters an '=' character, it increments i by 1.
	return the final value of i, the length of the substring.
*/
static int	determine_token_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '<' || str[i] == '>' || str[i] == '=' || str[i] == '|')
	{
		if (str[i] == '>' && str[i + 1] == '>')
			i = 2;
		else
			i = 1;
	}
	else
	{
		while (str[i] && !ft_isspace(str[i]) && str[i] != '<' && str[i] != '>'
			&& str[i] != '=' && str[i] != '|')
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




/*
	counting the number of arguments in a given string str
	loop thorough 'str'
	skip white spaces
	the counter 'n' counts the number of arguments
	advance the string pointer by the length of the current argument
	skip white spaces again
	return the total count of arguments 'n'
*/
int	count_cmdline_args(char *str)
{
	int	n;

	n = 0;
	ft_skip_spaces(&str);
	while (*str)
	{
		ft_skip_spaces(&str);
		n++;
		str += determine_token_len(str);
		ft_skip_spaces(&str);
	}
	return (n);
}




/*
	extract and set individual arguments from a given string
	into an array while also removing certain tokens or processing the string
	skip white spaces
	loop until the end of argc
	calculate the len of each elements
	duplicate each elements and save it in argv array
	the remove_token check to '' "" and \ and removes it
	increament the i value
	increament the pointer str by the len of that character
*/
void	parse_and_assign_args(char **argv, char *str, int argc)
{
	int	i;
	int	len;

	i = 0;
	ft_skip_spaces(&str);
	while (i < argc)
	{
		ft_skip_spaces(&str);
		len = determine_token_len(str);
		argv[i] = ft_strldup(str, len);
		remove_token(&(argv[i]));
		i++;
		str += len;
		ft_skip_spaces(&str);
	}
}




/*
	check the value of the errno variable and handle specific error conditions

	errno-base.h
	#define	ENOENT			2	No such file or directory
	#define	EACCES		13	Permission denied

	if errno is ENOENT or EACCES print error message
	fd 2 stderr
	set data->ret either to 127 or 126
	127 for ENOENT
	126 for EACCES
	return (1) to indicate an error

	else if errno is not ENOENT OR EACCES retrun (0)
*/
static int	file_access_error(t_data *data, char *str)
{
	if (errno == ENOENT || errno == EACCES)
	{
		ft_putstrs_fd("bash: ", str, ": ", 2);
		ft_putstrs_fd(strerror(errno), "\n", 0, 2);
		if (errno == ENOENT)
			data->ret = 127;
		else
			data->ret = 126;
		return (1);
	}
	return (0);
}




/*
	to check the type of a file or directory,
	handle cases where the path is a file or a directory

	If an error is detected, return early
	else check the path type, if not a dir
	open the file and parse it's contents
	free the data struct string
	read and proccess the file line by line
	and parsing each line as a cmd
	and store the result in the data->cmds

	If the path is indeed a directory, it prints an error message and sets
	the return code (data->ret) to indicate that the path is a directory.
*/
static void	handle_path_type(t_data *data, char *str, char *path)
{
	DIR		*dir;
	int		fd;
	char	**cmds;

	if (file_access_error(data, str))
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




/*
	to modify a path by navigating to a parent directory based on "../"
	extract a filename from str and then adjust the path accordingly

	*filename;			Extracted filename
	*parentPath;		Temporary storage for parent directory
	*modifiedPath;		Modified path result

*/
static void	set_modified_path(int len, char **new, char *str)
{
	int		i;
	char	*filename;
	char	*parentPath;
	char	*modifiedPath;

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
	parentPath = ft_strldup(*new, len);
	free(*new);
	modifiedPath = ft_strjoin(parentPath, "/");
	free(parentPath);
	parentPath = ft_strjoin(modifiedPath, filename);
	free(modifiedPath);
	free(filename);
	*new = parentPath;
}




/*
	modify a path by either replacing it with a new path specified in
	the str argument or adjusting the current path based on str

	check if str starts with a "/" meaning absolute path
	if so, it replaces the path with the str.
	If str doesn't start with a forward slash, meaning it's not relative path
	it calls a set_modified_path to adjust the current path by combining it with str.
	to adjusts the newPath, and assigns the modified newPath to *path.
	and frees the temporary newPath.
*/
static void	set_path(char *str, char **path)
{
	int		len;
	char	*newpath;

	newpath = ft_strdup(*path);
	len = ft_strlen(*path);
	if (!ft_memcmp(str, "/", 1))
		*path = ft_strdup(str);
	else
	{
		set_modified_path(len, &newpath, str);
		*path = newpath;
		return ;
	}
	free(newpath);
}




/*
	it executes a bash cmd and manages the processes

	Store the original argv[0] for later restoration
	Remove leading "./" from the cmd if it exists
	Get the current working directory
	Set the path in the argv[0] if needed
	Create a child process
	setup child proccess
	Execute the cmd
	Exit the child process
	Parent process waits for the child
	Normalize the return value
	Free allocated memory
	Restore the original argv[0]
*/
void	exec_bash_cmd_with_fork(t_data *data)
{
	char	buff[4097];
	char	*path;
	char	*argv_zero;

	argv_zero = data->argv[0];
	//if (ft_memcmp(data->argv[0], "/", 1))
	//	data->argv[0] += (!ft_memcmp(data->argv[0], "./", 2)) ? 2 : 0;
	if (data->argv[0][0] == '.' && data->argv[0][1] == '/')
		data->argv[0] += 2;
	path = getcwd(buff, 4096);
	set_path(data->argv[0], &path);
	if (!fork())
	{
		signal(SIGINT, child_sig_handler_bash);
		if (execve(path, data->argv, data->envp))
			handle_path_type(data, argv_zero, path);
		exit(data->ret);
	}
	else
		wait(&data->ret);
	data->ret /= 256;
	free(path);
	data->argv[0] = argv_zero;
}





/*
	It is responsible to processes environment variables
	Check if the cmd is "export" and has no arguments (argc == 1 and empty argv[1])
	It sort the environment variables data->envp and exports data->export by '=' and 0 criteria

	loop thorough each element of argv "i = 1" starting from second argv
	check_export_error checks if there's an error while proccessing the current argv
	if error increament 'data->ret'

	else if the cmd is export (data->argv[0]) start with export value
	else if the commadn is unset start with unset, to remove an env calling
	unset_cmd and increament i value

	after proccessing all arguments, if error set data->ret to 1, else to 0
	and return the updated environment variables
*/
static char	**proccess_env(t_data *data, int fd)
{
	int	i;

	data->ret = 0;
	if (!ft_memcmp(data->argv[0], "export", 7) && data->argc == 1 && !ft_strlen(data->argv[1]))
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
				data->envp = unset_cmd(data, i++);
		}
	}
	data->ret = data->ret ? 1 : 0;
	return (data->envp);
}




/*
	To Handle the "env" cmd. If the cmd has no additional arguments

	Check if the number of arguments is not equal to 1, if there's more than one arg
	Print an error message indicating permission denied and set the return value to 126

	If there's only one argument, which is "env," enters a loop to iterate thorough the
	environment variables
	inside the loop
	print each environment variables followed by newline to the specified file descriptor

*/
static void	env_cmd(t_data *data, int fd)
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





/*
	it handles the echo cmd.
	it check whether to skip the first argument ("-n") and then print
	the remaining arguments to a specified file descriptor, and adds
	a space between non-empty arguments

	sets i value either to 1 or 0 depending on if ("-n") is present

	loops thorough each argument starting from actual i value
	inside the loop prints the current argument
	checks if it's the last argument and the next argument is not empty
	if so print a space

	outside the loop it checks if the '-n' option is not present, print a newline character

*/
static void	echo_cmd(t_data *data, int fd)
{
	int	i;

	if (data->argc > 1 && !ft_memcmp(data->argv[1], "-n", 3))
		i = 1;
	else
		i = 0;
	while (++i < data->argc)
	{
		ft_putstr_fd(data->argv[i], fd);
		if (i < data->argc - 1 && ft_strlen(data->argv[i + 1]))
			write(fd, " ", 1);
	}
	if (!(data->argc > 1 && !ft_memcmp(data->argv[1], "-n", 3)))
		write(fd, "\n", 1);
}





/*
	check if the given cmd is a built-in cmd, execute the corresponding function.
	If the cmd is not recognized as a built-in, return 1 to indicate that it's an external cmd.
	handle built-in cmds for echoing, printing the current working directory, and changing the directory.

	first check if cmd is echo
	if so call the func echo_cmd

	if cmd is pwd, get the cwd and print it to the specified file descriptor

	if cmd is cd, duplicate the dir path from argv[1] and store it in path
	call the cd_cmd func to change the cwd
	check if path is set and if it's a "-" (indicating switching to previous dir)
	if so get the cwd and print if to the specified fd
	free pointer path

*/
static int	check_builti(int fd, t_data *data)
{
	char	*path;
	char	cwd[4097];

	path = NULL;
	if (!ft_memcmp(data->argv[0], "echo", 5))
		echo_cmd(data, fd);
	else if (!ft_memcmp(data->argv[0], "pwd", 4))
		ft_putstrs_fd(getcwd(cwd, 4096), "\n", 0, fd);
	else if (!ft_memcmp(data->argv[0], "cd", 3))
	{
		path = ft_strdup(data->argv[1]);
		if (!path)
			return (1);
		cd_cmd(data);
		if (path && !ft_strncmp(path, "-", 2))
			ft_putstrs_fd(getcwd(cwd, 4096), "\n", 0, fd);
		free(path);
	}
	else
		return (1);
	return (0);
}





/*
	checks various built-in cmds and execute the corresponding functions or return an exit code.
	checking what builti function returns if false return ret value
	else if builti returns true, then it checks the argv[0] if it's "env", call the env_cmd func
	else if the argv[0] starts with "./", "../", or "/", indicating that it's a cmd to execute
	a local file. If any of these conditions are true, it calls the exec_bash_cmd_with_fork

	else if argv[0] in data->argv is "export" (the first 7 characters) or "unset" (the first 6 characters).
	call the proccess_env function and assigns the result to data->envp.
	else if argv[0] in data->argv is "exit" (the first 5 characters) or "q" (the first 2 characters).
	call the exit_cmd function.
	else retrun (127) If none of the above conditions are met, an unkown cmd error
	and return data->ret
*/
int	check_builtins(int fd, t_data *data)
{
	data->ret = 0;
	if (!check_builti(fd, data))
		return (data->ret);
	else if (!ft_memcmp(data->argv[0], "env", 4))
		env_cmd(data, fd);
	else if (!ft_memcmp(data->argv[0], "./", 2) || !ft_memcmp(data->argv[0], "../", 3) || !ft_memcmp(data->argv[0], "/", 1))
		exec_bash_cmd_with_fork(data);
	else if (!ft_memcmp(data->argv[0], "export", 7) || !ft_memcmp(data->argv[0], "unset", 6))
		data->envp = proccess_env(data, fd);
	else if (!ft_memcmp(data->argv[0], "exit", 5) || !ft_memcmp(data->argv[0], "q", 2))
		exit_cmd(data);
	else
		return (127);
	return (data->ret);
}




/*
	changes the current directory, updates the environment variables OLDPWD and PWD,
	and handles error reporting if the directory change operation fails.

	getcwd(oldpwd, 4097)
	To store the current working directory to the old working directory.
	if chdir(path) returns 0 -> (change dir to the path) indicating success
	if changing dir is successful go inside if statement
	*	argc is set to 4 indicating 4 arguments
	*	argv is freed by calling free matrix
	*	new argv is allocated with size 4 char *
	*	argv[0] is set to "export"
	*	argv[1] is set to "OLDPWD="
	*	argv[2] is set to oldpwd
	*	the envp is updated by calling export_cmd
	the data->argv array is freed again
	a new data is allocated with size 4 char *
	and argvs are set accordingly

	if changing dir fails chdir is non zero
	an error message to fd 2
*/
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
		if (!data->argv[0])
			return ;
		data->argv[1] = ft_strdup("OLDPWD=");
		if (!data->argv[1])
			return ;
		data->argv[2] = ft_strdup(oldpwd);
		if (!data->argv[2])
			return ;
		data->envp = export_cmd(data, 1);
		free_matrix(data->argv);
		data->argv = (char **)ft_calloc(sizeof(char *), 4);
		data->argv[0] = ft_strdup("export");
		if (!data->argv[0])
			return ;
		data->argv[1] = ft_strdup("PWD=");
		if (!data->argv[1])
			return ;
		data->argv[2] = ft_strdup(getcwd(cwd, 4096));
		if (!data->argv[2])
			return ;
		data->envp = export_cmd(data, 1);
	}
	else
		ft_putstrs_fd("bash: cd: ", data->argv[1], ": ", 2);
}




/*
	handling the "cd" (change directory) cmd
	if argc is <= 2 proceeds with the directory change.
	If there are more than two arguments, it prints an error message
	Within the condition for two or fewer arguments:
	*	check if argv[1] is NULL starts with "--" or "~", or is "-"
		Depending on these conditions, it sets the path variable to the home directory ("HOME" environment variable),
		the previous directory ("OLDPWD" environment variable),
		change_dir function with the calculated path and data as arguments to change the current directory.
		If errno is greater than 0, it means that an error occurred during the directory change.
		In this case, it prints an error message using strerror(errno)
		If there are more than two arguments, it prints an error message indicating
		that there are too many arguments and sets data->ret to 1 to indicate an error.
*/
void	cd_cmd(t_data *data)
{
	char	*path;

	path = NULL;
	errno = 0;
	if (data->argc <= 2)
	{
		if (!data->argv[1] || !ft_strncmp(data->argv[1], "--", 3) || !ft_strncmp(data->argv[1], "~", 2))
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
	else if (data->argc == 1)
	{
		rl_on_new_line();
	}
	else
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		data->ret = 1;
	}
	errno = 0;
}




/*
	setting up input redirection
	to redirect input from a file to the standard input
	loops thorough argv array to find the redirection symbol "<"
	or reaches the end of the array. ft_memcmp compares strings,
	and it looks for a match with "<" (the first 2 characters).
	If it finds the "<" symbol (i.e., argv[i] is not NULL),
	it means that input redirection is specified in the cmd line.
	open a file specified after the "<" symbol, which is argv[i + 1],
	for reading (O_RDONLY) with read permissions (0666).
	if fd < 0 print error msg and return
	else if the file is opended redirect the input
	dup2 to duplicate the fd (opened file) into fd 0
	which is the stdin, to redirect the standard input to the file
	and close the fd

*/
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




/*
	execute external binaries that is located at a specified path,
	handle input and output redirection,
	and manage error conditions, including permission issues.
	fork-exec-wait to run the external program in a separate
	child process, and provides an exit status based on the success
	or failure of the execution

	make a copy of cmd arg's
	handle SIGINT and handle keyboard interrupts in the child proccess
	create a child proccess to execute the external binary in the child proccess
	set_in(); to handle redirections
	check fd value to duplicate fd into stdout (handle output redirection)
	execute the external binary located at path with the provided args and envp
	if exec fails it returns a non zero, and the error related to permission (error code 'EACCES')
	it sets 'data->ret' to 126 indicating permissin issue.
	print an error message to the stderr (file descriptor 2) that includes the cmd name,
	the error msg (obtained from strerror(errno)), and sets data->ret accordingly.
	exit(data->ret); It exits the child process with the value of data->ret.

	In the parent proccess it waits for the child process to complete
	and collects its exit status, which is stored in data->ret.
	divide data->ret by 256. to convert the exit status to the format
	typically used in shell scripts, an exit status of 0 indicates success.
	free the memory allocated for the path (which is the binary)
	free the memory allocated for the args array,
	which holds a copy of the cmd's arguments.
*/
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




/*
	to split the "PATH" environment variable into individual directory paths.
	in the context of searching for an executable binary in multiple directories.

	**paths to store the individual directory paths when splitting the "PATH" environment variable.
	get_env func to retrieve the value of the "PATH" from the data->envp array.
	if path is not NULL, if "PATH"  is set, if true,
	split the "PATH" variable based on the colon (':') delimiter

	else if 'path' is NULL, "PATH" is not set
	prints an error message to the stderr (file descriptor 2)
	set data->ret to 127 to indicate an error and returns NULL.
	If the "PATH" variable is set, return the array of directory paths obtained by splitting it.
*/
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




/*
	to searche for an executable binary in directories specified by the "PATH"
	and return the full path of the first match found. If no match is found, it return NULL.

	**paths	to store the individual directory paths obtained by splitting the "PATH"
	*path	to store the full path of the found executable.
	split_path func, to obtain the directory paths from the "PATH"
	If the function returns NULL (indicating an error), return NULL.
	iterates through the directory paths
	using opendir to open the current directory in the iteration and
	store the directory stream pointer in *dir.
	iterate through the directory entries using readdir.
	The loop continues as long as the directory stream is open ((*dir) is not NULL)
	and there are no permission issues (errno != EACCES)
	and there are more directory entries ((*d = readdir(*dir))).
	ft_memcmp compares the current directory entry's name ((*d)->d_name) with the target string str.
	If there is a match, it means the executable file is found.
	ft_strjoin to concatenate the directory path with a '/' character
	to create the full path of the executable.
	free the memory allocated for the array of directory paths.
	return the full path of the found executable
	After the loop, close the directory stream of the last directory.
	free the memory allocated for the array of directory paths if no match is found.
	If no executable is found in any of the directories, it returns NULL.
*/
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




/*
	to check for the existence of an executable binary file specified by data->argv[0].
	If the binary is found, execute it using the exec_bin function.

	*dir		a directory stream
	*d			a directory entry structure
	*pre_path	a prefix path
	*path		the full path of the executable

	initialize the return value data->ret to 127.
	This value suggests that the binary file was not found.
	call the search_bin function to find the executable file.
	The search_bin function returns a prefix path (pre_path)
	if the executable is found.
	if pre_path is not NULL, if the executable was found in one of the directories.
	strjoin to concatenate the prefix path with the name of the executable
	to create the full path of the executable (path).
	call the exec_bin function to execute the binary.
	fd argument is used for handling input/output redirection.
	close the directory stream.
	free the memory allocated for the prefix path (pre_path),
	regardless of whether the binary was found.
	return the value of data->ret,
	which is set by the exec_bin function to indicate
	the success or failure of executing the binary.
*/
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




/*
	to handle output redirection by opening and managing file descriptors based on
	the presence of ">" and ">>" in the cmd arguments.
	take care of creating, truncating, and appending to files as specified,
	and handle potential issues with file readability during the append operation.

	iterate through the cmd arguments.
	check if the current argument indicates output redirection using ">".
	If true, open the specified file for writing, creating the file
	if it doesn't exist (O_CREAT) and truncating it to zero length (O_TRUNC).
	permissions are set to 0666.
	update the fd to the new file.
	else if the current argument indicates append redirection using ">>",
	open the specified file for writing, creating the file
	if it doesn't exist (O_CREAT) and appending to it (O_APPEND).
	checks if the file is readable, read a character from it.
	If there is an issue reading the file (e.g., due to permissions),
	print an error message.
	checks if there are more arguments, and if the next argument also
	indicates output redirection ("<<" or ">").
	If true, it closes the current file descriptor to prepare for the next redirection.
	return the file descriptor after completing the redirection operations.

*/
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
		if (data->argv[i] && (!ft_memcmp(data->argv[i], ">>", 3) || !ft_memcmp(data->argv[i], ">", 2)))
			close(fd);
	}
	return (fd);
}



/*
	responsible for setting up file descriptors for output redirection
	iterate through the cmd arguments (data->argv) to determine if
	output redirection (">" or ">>") is specified and,
	if so, call the redirect function to handle the redirection

	iterate through the cmd arguments until an argument containing ">" or ">>" found
	to determine whether output redirection is specified and to
	find the position of the redirection in the argument list
	If the loop reaches the end of the argument list and does not find ">" or ">>",
	it means there is no output redirection specified,
	and it returns the default file descriptor for standard output (1).
	If output redirection is specified,
	call the redirect function to handle the redirection
	passe the data structure, the index i indicating the position of the redirection
	in the argument list, and the default file descriptor fd.
*/
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



/*
	to count the number of output redirections specified in the cmd arguments (data->argv).
	It iterates through the arguments and increments the count whenever it encounters ">" or ">>".

	iterates through the cmd arguments.
	checks if the current argument contains ">" or ">>". If true,
	increment the count variable and increment i by 1 to skip
	the next argument (the file name after ">" or ">>").
	return the final count of output redirections.
*/
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



/*
	create a new array of cmd arguments without considering output
	redirection symbols (">" and ">>"), updating the data->argc and data->argv

	update the data->argc variable by subtracting twice the count of output redirections.
	This adjustment is made because each output redirection symbol (">" or ">>") is
	followed by a file name, and these pairs are removed from the argument count.

	allocate memory for the new arguments array (args) based on the adjusted argument count.
	i = 0; j = 0;: initialize for iterating through the original and new arguments arrays.
	iterates through the original cmd arguments and fills the new arguments array.

	checks if the current argument contains ">" or ">>".
	If true, increment the index i by 2 to skip the file name that
	follows the output redirection symbol.
	If the current argument is not an output redirection symbol,
	duplicate the argument using ft_strdup and assigns it to the new arguments array (args).
	increment both i and j.
	free the memory allocated for the original arguments array.
	update data->argv to point to the newly created arguments array.
*/
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



/*
	handle a cmd by setting up output redirection,
	create a new array of cmd arguments,
	check if the cmd is a built-in function or an external binary,
	and printing an error message if the cmd is not found.

	check if the argument zero in the cmd (the cmd itself) is not NULL
	and not an empty string.
	fd = set_fd(data); call the set_fd function to set up
	the file descriptor for output redirection.
	copy_args1 func will create a new array of cmd arguments
	without considering output redirection symbols.
	check_builtins function will check if the cmd corresponds
	to a built-in function. The result is stored in data->ret.
	checks if the return value is 127
	(indicating that the cmd is not a built-in function)
	check_bin function will check if the cmd corresponds to an external binary.
	If the return value is still 127, it means the cmd is not found.
	print an error message
	If the file descriptor is not the default standard output (1),
	close the file descriptor.
	return the pointer to the environment variables (data->envp).
*/
char	**check_cmd(char *str, t_data *data)
{
	int	fd;

	if (data->argv[0] && *(data->argv[0]))
	{
		fd = set_fd(data);
		copy_args1(data);
		data->ret = check_builtins(fd, data);
		if (data->ret == 127 && (data->ret = check_bin(fd, data)) == 127)
		{
			//if (ft_strncmp(str, "cd", 3) == 0)
			//{
			//	ioctl(STDIN_FILENO, TIOCSTI, "\n");
			//	//rl_on_new_line();
			//}
			//else
			//{
				ft_putstrs_fd(0, str, ": cmd not found.\n", 2);
				data->ret = 127;
			//}
		}
		if (fd != 1)
			close(fd);
	}
	return (data->envp);
}



/*
	to ensure that the memory allocated for various fields in the t_data structure
	is properly freed, preventing memory leaks.
	when the t_data structure is no longer needed,
	typically when the program is finished using it or when cleaning up resources.
*/
static void	free_param(t_data *data)
{
	free(data->str);
	free_matrix(data->envp);
	free_matrix(data->export);
	free_matrix(data->argv);
	free_matrix(data->cmds);
	free(data);
}




/*
	It handles the exit cmd by checking the number and validity of arguments.
	If there are too many arguments or the second argument is not a valid numeric value,
	it prints an error message. Otherwise, it frees the allocated memory and exits
	the program with the specified exit code.

	checks if there are more than two arguments
	(the cmd itself and a numeric argument, if present).
	If true, it prints an error message and sets data->ret to 1.
	prints an error msg. and set data->ret to 1.
	else if there are two or fewer arguments:
	set i to 0, and iterates through the characters of the second argument
	(if present) to check if it consists of digits.
	"if (data->argc > 1 && data->argv[1][i])"
	If the second argument is present and contains non-digit characters,
	print an error msg and sets data->ret to 2. the error msg indicates that
	a numeric argument is required for the exit cmd

	set i to the integer value of the second argument using ft_atoi
	if it is present and does not contain non-digit characters.
	Otherwise, it set i to data->ret

	free the memory allocated for the t_data structure and its fields using the free_param function.
	exit the program with the exit code i.
*/
void	exit_cmd(t_data *data)
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
			ft_putstrs_fd("exit\nbash: exit: ", data->argv[1], ": numeric argument required\n", 2);
			data->ret = 2;
		}
		i = (data->argc > 1 && data->ret != 2) ? ft_atoi(data->argv[1]) : data->ret;
		free_param(data);
		exit(i);
	}
}




/*
	It handles the export cmd by updating the environment variables based on the provided arguments.
	If the specified variable is not found, it creates a copy of the current environment variables and
	adds the new variable. If the variable is found, it updates the existing variable.
	The function returns a pointer to the updated environment variables.

	iterate through the current environment variables to find a match for the specified variable (data->argv[j]).
	If the specified variable is not found in the current environment variables:
	create a copy of the current environment variables using the copy_env function.
	strjoin to concatenates the variable and its value into a single string and
	assigns it to the corresponding position in the copied environment variables.
	free the memory
	If the specified variable is found in the current environment variables:
		assign the pointer to the current environment variables to cpy.
	free the memory
	data->envp[i] = ft_strjoin(data->argv[j], data->argv[j + 1]);
	concatenates the variable and its value into a single string and assigns
	it to the corresponding position in the current environment variables.
	return the pointer to the updated environment variables.
*/
char	**export_cmd(t_data *data, int j)
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




/*
	It handles the processing of values in the export cmd.
	If the current argument is a variable without a value,
	it adds it to the export variables. If the argument is a variable with a value,
	it calls the export_cmd function to handle the export cmd and updates the
	environment variables accordingly.
	The function increments the index i to continue processing the next arguments.
	check if the current argument does not contain the '=' character,
	suggesting that it's a variable without a value.
	iterate through the existing export variables to find a match for the specified variable (data->argv[*i]).
	If the specified variable is not found in the existing export variables
	create a copy of the existing export variables using the copy_env function.
	duplicate the specified variable and assigns it to the corresponding position in the copied export variables.
	set the next position to NULL to mark the end of the array.
	free the memory
	assign the pointer to the copied export variables to data->export.
	increments the index i.

	If the current argument contains the '=' character, suggesting that it's a variable with a value
	call the export_cmd function to handle the export cmd for the specified variable
	and its value. and update the environment variables.
	increment the index i based on whether there is a value after the current argument
	(increments by 2 if a value is present, otherwise by 1).

*/
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
		data->envp = export_cmd(data, *i);
		*i += data->argv[*i + 1] ? 2 : 1;
	}
}




/*
	checks for errors in the arguments for the export cmd.
	It verifies that the arguments follow the rules for valid identifiers
	and prints an error msg if an issue is detected.
	returns 1 if an error is detected and 0 otherwise.

	iterate through the characters of the current argument,
	checking if each character is an alphanumeric character,
	underscore, hash symbol, equals sign, or dollar sign.

	if the first character of the current argument is a digit or
	if there are characters after the valid ones. If true, an error is detected.

	If the current argument contains the equals sign ('='),
	it prints an error message indicating that it's not a valid identifier,
	and increments i to move to the next argument.
	print an error prefix.
	ft_putstrs_fd(argv[*i], argv[(*i) + 1], "': not a valid identifier\n", 2);
	prints the specific part of the argument causing the error.

	else if the current argument does not contain the equals sign:
	print an error prefix and specific part of the argument causing the erro
	returns 1 -> fail
	or returns 0 success
*/
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
			ft_putstrs_fd(argv[*i], argv[(*i) + 1], "': not a valid identifier\n", 2);
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



/*
	To format and output environment variables in the "declare -x" format,
	including the variable value enclosed in double quotes.
	The output is directed to the specified file descriptor (fd).
	The function is static, suggesting that it is intended for use within the same source file.

	iterate through the environment variables.
	write the string "declare -x " to the specified file descriptor.
	write a substring of env_var[i] up to the position of
	the equals sign ('=') plus one.
	check if the current environment variable contains an equals sign ('=').
	If true, write the value of the environment variable
	(substring after the equals sign) enclosed in double quotes.
	write a newline character to the specified file descriptor,
	indicating the end of the current environment variable entry.
	free the memory
*/
static void	put_envp(char **env_var, int fd)
{
	int	i;

	i = -1;
	while (env_var[++i])
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstrlen_fd(env_var[i], ft_strlen_char(env_var[i], '=') + 1, fd);
		if (ft_strchr(env_var[i], '='))
			ft_putstrs_fd("\"", ft_strchr(env_var[i], '=') + 1, "\"", fd);
		write(fd, "\n", 1);
	}
	free_matrix(env_var);
}




/*
	It sorts the environment variables lexicographically based on a substring up to the first
	occurrence of the specified character c. It then outputs the sorted environment variables
	using the put_envp function. The env_var array is used for sorting,
	and memory is freed after the sorting and output are completed.

	create a copy of the original environment variables
	iterate through the env_var array, representing environment variables
	calculate the length up to the first occurrence of character c in the current
	and next environment variables. It determines the length based on the shorter of the two.

	compare the substrings of the current and next environment variables up to the length len.
	If the comparison is greater than 0, it means the current variable comes after the next variable lexicographically.
	swap the positions of the current and next environment variables
	in the env_var array, resetting the index i to -1 to start the comparison again.
	The put_envp function outputs the sorted environment variables to the specified file descriptor.
*/
void	sort_envp(char **envp, int fd, char c)
{
	int		i;
	int		len;
	char	**env_var;
	char	*tmp;

	env_var = copy_env(envp, 0);
	i = 0;
	while (env_var[i] && env_var[i + 1])
	{
		len = (ft_strlen_char(env_var[i], c) > ft_strlen_char(env_var[i + 1], c))
			? ft_strlen_char(env_var[i], c)
			: ft_strlen_char(env_var[i + 1], c);
		if (ft_memcmp(env_var[i], env_var[i + 1], len) > 0)
		{
			tmp = env_var[i];
			env_var[i] = env_var[i + 1];
			env_var[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
	put_envp(env_var, fd);
	free_matrix(env_var);
}



/*
	To calculates the length of a substring in a string until the first occurrence of the pipe character ('|'),
	considering quotes and escape characters.
	If the quotes are not properly closed, an error message is printed,
	and the function returns the current index. Otherwise, it returns the length of the substring.

	iterate through the string until it reaches the pipe character ('|') or the end of the string.
	If the current character is a double or single quote
	It sets the variable c to the quote character and increments the index i to move past the quote.
	The second loop iterates through the characters until it finds the matching quote character (c).
	If the current character is a backslash ('') and the next character is a token (special character),
	it skips the next character (escaping).
	If the end of the string is reached and a matching quote is not found
	print an error message and return the current index i.
	if the current character is a backslash (''), it skips the next character (escaping).
	and returns the final index i, representing the length of the substring.
*/
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




/*
	To calculate the length of a substring in a string until the first occurrence of a specified token character (c).
	It considers escape characters ('') before token characters to correctly determine the length of the substring.
	The function then returns the length of the substring.

	iterate through the string until it reaches the token character (c) or the end of the string.
	If the current character is a backslash ('') and the next character is a token (special character),
	skip the next character (escaping).
	return the final index i, the length of the substring.
*/
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




/*
	To calculates the length of a substring in a string until the first occurrence of a specified character (c).
	It counts the number of characters in the substring until the target character is encountered
	or the end of the string is reached, and then returns the length of the substring.

	iterate through the string until it reaches the target character (c) or the end of the string.
	return the final index i, the length of the substring.
*/
int	ft_strlen_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}




/*
	To calculates the length of a token or a substring in a string until the first occurrence of specific characters,
	considering various characters as potential separators.
	The function increments the length for each character in the token and returns the total length.
	If the last character is an equal sign, an additional increment is performed.

	iterate through the string until it reaches one of the specified characters
	(space, semicolon, pipe, less-than, greater-than, double and single quotes, equal sign)
	or the end of the string.
	len++; str++;: For each character that is not one of the specified separators,
	it increments the len variable and moves the pointer str to the next character.

	If the last character encountered is an equal sign ('='), it increments the len variable.
	return the final value of len, the length of the token or substring.
*/
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




/*
	To calculates the length of an environment variable name or a substring in a string until
	the first occurrence of a character that is not alphanumeric,
	an opening curly brace ('{'), the question mark ('?'), or the underscore ('_').
	It increments the length for each valid character and, if the last character is an equal sign,
	performs an additional increment before returning the total length.

	iterate through the string until it reaches a character that is not alphanumeric,
	an opening curly brace ('{'), the question mark ('?'), or the underscore ('_'), or the end of the string.
	For each valid character in the environment variable name,
	it increments the len variable and moves the pointer str to the next character.
	If the last character encountered is an equal sign ('='), it increments the len variable.
	return the final value of len, the length of the environment variable name or substring.
*/
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




/*
	replaces specific patterns in a string with corresponding environment variable values,
	considering different cases such as the presence of braces or the "?" character.
	It returns the length of the replaced environment variable.

	set the value of braces based on whether the character following the current index is an opening curly brace ('{').

	calculate the length of the substring that represents the environment variable to be replaced.
	The length is determined by either the length until the first colon (':')
	or the length until the first character that is not alphanumeric, '{', '?', or '_'.
	The calculation also takes into account the presence of braces.

	bef = ft_strldup(*str, i);
	creates a substring (bef) from the beginning of the original string up to the index i.

	aux = ft_strldup(*str + i + 1 + braces, len - 1 - braces * 2);
	creates a substring (aux) representing the environment variable pattern to be replaced.

	env = (!ft_memcmp(aux, "?", 2)) ? ft_itoa(data->ret) : 0;
	If aux is equal to "?", it sets env to the string representation of data->ret
	using ft_itoa. Otherwise, it sets env to 0.

	aft = ft_strdup(*str + i + len);
	creates a substring (aft) from the original string starting from the position after the environment variable pattern.

	env = (!env) ? ft_strdup(get_env(data->envp, aux)) : env;
	If env is still 0 (indicating that it's not set by the "?" case),
	it sets env to the value of the corresponding environment variable using get_env.

	len = ft_strlen(env);
	It updates the length to the length of the replacement value.

	aux = ft_strjoin(bef, env);
	concatenates the substring before the environment
	variable (bef) with the environment variable value (env).

	free memory allocated for bef, env, and the original string.
	strjoin  sets *str to the result of concatenating aux with aft.

	free memory allocated for aux and aft.
	return the length of the replaced environment variable.
*/
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



/*
	checks if single quotes in a string are properly closed.
	It increments the index past the current single quote and
	continues scanning the string until it finds another single quote
	or reaches the end of the string. If the end of the string is
	reached without finding the closing single quote, it prints an error message
	and returns 1. Otherwise, it returns 0 to indicate successful completion of single quotes.

	iterate through the string until it finds either another single quote or the end of the string.
	This effectively skips characters within the single quotes.

	If the end of the string is reached and no closing single quote is found:
	print an error message indicating that the quotes are not finished.
*/
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




/*
	scans a string for environment variable references and processes them.
	It also handles special cases involving single quotes and escaping characters.
	If incomplete single quotes are detected, it returns 1; otherwise, it returns 0.

	initialize braces to 0, to determine whether the current environment variable is wrapped in curly braces.
	iterate through the string until it reaches the end.
	If a single quote is encountered, it calls the check_quotes function to ensure proper completion of the quotes.
	If incomplete quotes are detected, the function returns 1.
	if ((*str)[i] && (*str)[i] == '\\') { ... }:
	If a backslash is encountered, it checks for special cases:
		If the next character is a dollar sign ('$'),
		it removes the backslash. This likely deals with escaping the dollar sign.
	if ((*str)[i + 1]) i++;
	If there is a next character,
	it increments the index to skip the escaped character
	else if a dollar sign ('$') is encountered and it's not followed by whitespace,
	single quote, double quote, or a slash,
	it calls the change_env function to process the environment variable reference.
	If the function has not returned due to incomplete quotes, it returns 0
*/
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
				remove_char(str, i);
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




/*
	checks whether the shell cmd string in data->str starts with a semicolon or is null.
	If it does, it prints an error message, sets the return value in data->ret to 2,
	frees the memory allocated for data->str, and returns 1.
	Otherwise, it returns 0 to indicate that no syntax error involving semicolons was detected.

	check whether data->str is null or starts with a semicolon.
	If data->str is not null, it prints an error message indicating a syntax error near unexpected token ; and sets data->ret to 2.

	free the memory allocated for data->str and sets it to null.
	return 1 to indicate that a syntax error involving semicolons was detected.

*/
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



/*
	performs parsing and execution of a shell cmd,
	handling semicolons, environment variable substitutions,
	and individual cmds or pipelines. It utilizes helper functions like check_semicolon,
	check_env, count_cmdline_args, parse_and_assign_args, and cmd_or_pipe to achieve these tasks.

	checks for a syntax error involving semicolons using the check_semicolon function.
	If a syntax error is detected, the function returns early.

	split the shell cmd string into an array of strings using semicolon
	as delimiters and assigns the result to data->cmds.
	iterate through the array of cmds.

	check_env(&(data->cmds[i]), data);
	call the check_env function to perform environment variable substitutions in the current cmd.

	data->argc = count_cmdline_args(data->cmds[i]);
	count the number of cmd-line arguments in the current cmd and assigns the result to data->argc.

	data->argv = (char **)ft_calloc(sizeof(char *), (data->argc + 1));
	allocate memory for an array of cmd-line arguments plus a null terminator.

	parse_and_assign_args(data->argv, data->cmds[i], data->argc);
	parse the cmd string and assigns the parsed arguments to data->argv.

	cmd_or_pipe(data, i);
	call the cmd_or_pipe function to handle the execution of individual cmds
	or pipelines based on the parsed arguments.

	free the memory allocated for the array of cmd-line arguments.
	free the memory allocated for the original shell cmd string and sets it to null.
	free the memory allocated for the array of cmds.
*/
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
		data->argc = count_cmdline_args(data->cmds[i]);
		data->argv = (char **)ft_calloc(sizeof(char *), (data->argc + 1));
		parse_and_assign_args(data->argv, data->cmds[i], data->argc);
		cmd_or_pipe(data, i);
		i++;
		free_matrix(data->argv);
	}
	free(data->str);
	data->str = 0;
	free_matrix(data->cmds);
}



/*
	extracts the arguments for a cmd within a pipeline by iterating through
	the original array of arguments (data->argv) starting from the specified index i
	until it encounters a pipe character (|) or reaches the end of the arguments.
	It then allocates a new array (aux) and copies the relevant arguments into it,
	updating data->argv and data->argc accordingly.

	iterates through the arguments starting from the specified index i until it encounters
	the end of the arguments or a pipe character (|). It increments j for each argument.

	allocate memory for an array of strings (aux) to store the arguments within the pipeline.

	copie the arguments from the original array data->argv to the newly allocated array aux.

	data->argv = aux;
	assign the new array aux to data->argv.
	data->argc = j;
	set data->argc to the number of arguments within the pipeline (j).
*/
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



/*
	is responsible for the execution of the child process in a pipeline.
	It sets up input/output redirection based on flags, closes unnecessary file descriptors,
	sets the arguments for the cmd within the pipeline,
	executes the cmd, and then exits the child process with the appropriate exit code.

	set up a signal handler for the child process to handle the SIGINT signal.
	forking a new process
	If the first flag is not set, duplicate the read end of the pipe (fds[0]) to the standard input (0)
	If the second flag is not set, duplicate the write end of the pipe (fds[3]) to the standard output (1).
	close all file descriptors in the original array fds to avoid resource leaks.

	argc = data->argc;
	argv = data->argv;
	save the original values of argc and argv from the data structure.

	set_pipe_args(data, pos);
	set the cmd arguments for the cmd within the pipeline.

	check_cmd(data->str, data);
	check and executes the cmd within the pipeline using the check_cmd function.

	free_matrix(data->argv);
	data->argc = argc;
	data->argv = argv;
	free the memory allocated for the modified arguments,
	restores the original argc and argv values in the data structure.
	exit(data->ret);
	exit the child process with the exit code stored in data->ret.
*/
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
		check_cmd(data->str, data);
		free_matrix(data->argv);
		data->argc = argc;
		data->argv = argv;
		exit(data->ret);
	}
}


/*
	closes the read and write ends of the current pipe,
	updates the file descriptors to use the read and write ends of the new pipe,
	and creates a new pipe for subsequent cmds in a pipeline.
	This function is typically used when setting up and switching between pipes in a series of cmds.

	close(fds[0]);
	close(fds[1]);
	close the read (fds[0]) and write (fds[1]) ends of the current pipe.

	fds[0] = fds[2];
	fds[1] = fds[3];
	update the read (fds[0]) and write (fds[1]) ends of the pipe to be the same as
	the new pipe's read (fds[2]) and write (fds[3]) ends, respectively.

	pipe(fds + 2);
	creates a new pipe and updates the array fds to store the read
	and write ends of the new pipe at indices 2 and 3, respectively.
*/
static void	switch_pipes(int *fds)
{
	close(fds[0]);
	close(fds[1]);
	fds[0] = fds[2];
	fds[1] = fds[3];
	pipe(fds + 2);
}


/*
	manages the execution of cmds within a pipeline.
	It iterates through the cmd arguments, sets up input/output redirection using flags,
	executes child processes for each set of cmds, and updates file descriptors for subsequent
	cmds in the pipeline. The function returns the total number of child processes created.

	int sons;	to keep track of the number of child processes (sons)
	int *flag;	an array of flags (flag) to control input/output redirection

	allocates memory for the flag array
	iterates through the cmd arguments
	The second loop counts the number of arguments until it encounters a pipe (|) or reaches the end of the arguments.

	It sets flag[1] to 1 if there are no more arguments after the current set (i.e., at the end of the pipeline).

	execute the child process for the current set of arguments within the pipeline using the
	pipe_son function and increments the count of child processes (sons)
	update flag[0] and switch the file descriptors for the next set of cmds
	in the pipeline using the switch_pipes function.

	update the index j to the next set of arguments in the pipeline.
	frees the memory allocated for the flag array
	and return the number of child processes created (sons).
*/
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
		while (data->argv[j + i] && ft_memcmp(data->argv[j + i], "|", 2))
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




/*
	determines whether to execute a single cmd or a pipeline of cmds based on the presence of the pipe character (|).
	It sets up pipes for the pipeline, executes the pipeline, waits for child processes to finish, and restores the standard input.

	int fds[4];		 an array of file descriptors (fds)
	int std_out;	standard output file descriptor (std_out),
	int sons;		number of child processes (sons),

	duplicate the standard input file descriptor (0) and store it in std_out.
	This is done to restore the standard input after command or pipeline execution.

	It finds the index where the pipe character (|) appears in the command arguments.
	if (!data->argv[i]) data->envp = check_command(data->cmds[j], data);
	If there is no pipe character in the command arguments, it means there is a single command.
	execute the command using the check_command function and update the environment variables.

	else If there is a pipe character and there are commands in the pipeline:
		create two pipes (fds and fds + 2) to set up the pipeline.
	sons = check_pipe(fds, data);
	call the check_pipe function to execute the pipeline and gets the number of child processes created.

	while (sons-- > 0)
	wait(&data->ret);
	wait for all child processes to finish and collects their exit status.

	data->ret /= 256;
	divide the exit status by 256 to get the actual exit code.
	close all file descriptors associated with the pipeline.

	dup2(std_out, 0);
	restore the standard input by duplicating the original standard input file descriptor stored in std_out.
*/
void	cmd_or_pipe(t_data *data, int j)
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
		data->envp = check_cmd(data->cmds[j], data);
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



/*
	removes a character at the specified index from a string by creating substrings before and after
	the specified index, concatenating them, and updating the original string.

	bef = ft_strldup(*str, j);
	create a substring bef containing the characters before the specified index j by duplicating the portion of the original string.

	aux = ft_strdup(*str + j + 1);
	create a substring aux containing the characters after the specified index j + 1 by duplicating the portion of the original string.

	free(*str);
	*str = ft_strjoin(bef, aux);
	free the memory occupied by the original string,
	and then concatenates the substrings bef and aux to form the modified string,
	updating the double pointer str.

	free the memory occupied by the substrings aux and bef.
*/
void	remove_char(char **str, int j)
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


/*
	iterates through the characters of a string starting from a specified index,
	removes backslashes that precede certain tokens,
	and stops processing when a double quote is encountered.

	iterate through the characters of the string starting from the specified index *i.

	If the current character is a backslash (\) and the next character is a token
	(as determined by the is_token function),
	remove the backslash by calling the remove_char function.

	else if the current character is a double quote ("), it breaks out of the loop.
	This suggests that the function is designed to stop processing if a double quote is encountered.
*/
static void	remove_backslash(char **arg, int *i)
{
	while ((*arg)[*i])
	{
		if ((*arg)[*i] == '\\' && is_token((*arg)[*i + 1]))
			remove_char(arg, *i);
		else if ((*arg)[*i] == '"')
			break ;
		(*i)++;
	}
}



/*
	Removes tokens from string 'arg' tokens are "single/double quote, backslash"
	loops thorough each element of 'arg'

	first checks for single quote and removes it
	if it's a single quote then it's a single quoted string
	the remove_char function removes the single quote
	i value will be increamented by the len of that character
	call again remove_char to remove the closing single quote

	secode checks for double quote and removes it
	remove_char function receives the string and the character to be removed
	remove_backslash, backslash is used as escaped character
	and remove the closing duoble quote

	else if the current character is a backslash and next
	character is either a token or a single quote ('),
	It removes the backslash by calling remove_char.

	If none of the above conditions are met,
	increment the index i to move to the next character.
*/
void	remove_token(char **arg)
{
	int	i;

	i = 0;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '\'')
		{
			remove_char(arg, i);
			i += ft_strlen_char(*arg + i, '\'');
			remove_char(arg, i);
		}
		else if ((*arg)[i] == '"')
		{
			remove_char(arg, i);
			remove_backslash(arg, &i);
			remove_char(arg, i);
		}
		else if (((*arg)[i] == '\\') && (is_token((*arg)[i + 1]) || (*arg)[i
					+ 1] == '\''))
			remove_char(arg, i++);
		else
			i++;
	}
}


/*
	creates a new array of strings by excluding the entry at the specified index from the original array.
	The original array is then freed, and the new array is returned.

	calculates the length of the original array by counting the number of non-null entries.

	allocate memory for the new array cpy using calloc and checks if the allocation is successful.
	If not, it returns NULL.

	copie the entries from the original array to the new array up to the specified index i.

	skip the entry at the specified index i and continues copying the remaining entries
	from the original array to the new array.

	free the memory occupied by the original array.
	return the new array with the specified entry removed.
*/
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


/*
	removes the specified environment variable from the original environment variable array
	and returns the modified array.
	If the specified environment variable is not found, it returns the original array unchanged.

	If the command has fewer than two arguments (meaning it lacks the environment variable to unset),
	return the original environment variable array.

	calculate the length of the specified environment variable and create a string (env)
	by concatenating the specified environment variable with an equal sign.

	searche for the specified environment variable in the original environment variable array.

	if (data->envp[i])
	cpy = erase_env(data->envp, i);
	If the specified environment variable is found,
	create a new array (cpy) by removing the entry at the found index.
	If not found, it returns the original environment variable array.

	free the memory occupied by the string env
	and return the new array (cpy) the modified environment variable array.
*/
char	**unset_cmd(t_data *data, int j)
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


/*
	handle SIGINT by writing a newline character to the standard error stream,
	effectively producing a new line in the child process when an interrupt signal is received.
	This is a common behavior to provide a clean and readable output when a user interrupts a running command in a terminal.
*/
void	child_sig_handler(int sig)
{
	if (sig == SIGINT)
		write(2, "\n", 1);
}



/*
	signal handler is designed to handle SIGINT in a child process.
	When the interrupt signal is received, it writes a newline character and then exits the child process
	with a status code of 0.
	To ensures that the child process terminates gracefully in response to a user interrupt.
*/
void	child_sig_handler_bash(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		exit(0);
	}
}



/*
	to retrieve the value of a specified environment variable by searching through an array of environment variables.
	It relies on string manipulation and comparisons to locate the desired variable and extract its value.
	The result is a pointer to the value of the specified environment variable, or 0 if the variable is not found.

	The input env is modified by concatenating it with the equal sign ("=").
	This prepares the string for comparison with the environment variables in envp.

	calculate the length of the modified env string.
	iterate through the array of environment variables (envp) until it finds an environment variable
	that starts with the modified env string. The comparison is performed up to the length of the modified env string.

	If a matching environment variable is found (envp[i] is not NULL), tmp is assigned a pointer to the value
	of the environment variable, excluding the part that matches the specified env.
	If no matching environment variable is found, tmp is set to 0.

	memory allocated for the modified env string is freed.
	return a pointer to the value of the specified environment variable.
*/
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



/*
	to create a copy of an array of strings representing environment variables.
	It allocates memory for the copy, duplicates each string from the original array,
	and returns a pointer to the copied array.
	The additional add parameter allows for flexibility in the size of the copied array.
	If the memory allocation fails, the function returns 0.

	calculate the number of strings in the input array envp by counting
	the elements until a NULL pointer is encountered.

	allocate memory for the copy (cpy) using ft_calloc.
	The allocated space is calculated based on the number of elements in envp plus the specified add value.
	If the memory allocation fails, the function returns 0.

	iterate through each element of the input array envp and creates a duplicate string for each element
	using ft_strdup. These duplicated strings are then stored in the copy array cpy.
	return a pointer to the copied array of environment variables.
*/
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



/*
	for identifying characters that might have special meanings or require special handling in certain contexts,
	such as when parsing or interpreting strings. In this case, it seems to be used in conjunction with other
	functions that process strings, particularly in the context of handling escape characters.
*/
int	is_token(char c)
{
	if (c == '"' || c == '\\')
		return (1);
	return (0);
}
