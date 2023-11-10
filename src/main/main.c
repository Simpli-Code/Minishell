/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chruhin <chruhin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 20:32:38 by chruhin           #+#    #+#             */
/*   Updated: 2023/11/09 23:08:06 by chruhin          ###   ########.fr       */
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
	snprintf(prompt, sizeof(prompt),
			"\033[1;32m%s@%s\033[0;0m:\033[1;34m%s\033[0;0m$ ", user, hostname,
			cwd);
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
			//free_args(args, j); SHOULD BE IMPLEMENTED
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
	and parsing each line as a command
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
	it executes a bash command and manages the processes

	Store the original argv[0] for later restoration
	Remove leading "./" from the command if it exists
	Get the current working directory
	Set the path in the argv[0] if needed
	Create a child process
	setup child proccess
	Execute the command
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
	Check if the command is "export" and has no arguments (argc == 1 and empty argv[1])
	It sort the environment variables data->envp and exports data->export by '=' and 0 criteria

	loop thorough each element of argv "i = 1" starting from second argv
	check_export_error checks if there's an error while proccessing the current argv
	if error increament 'data->ret'

	else if the command is export (data->argv[0]) start with export value
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
	To Handle the "env" command. If the command has no additional arguments

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
	it handles the echo command.
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
	check if the given command is a built-in command, execute the corresponding function.
	If the command is not recognized as a built-in, return 1 to indicate that it's an external command.
	handle built-in commands for echoing, printing the current working directory, and changing the directory.

	first check if command is echo
	if so call the func echo_cmd

	if command is pwd, get the cwd and print it to the specified file descriptor

	if command is cd, duplicate the dir path from argv[1] and store it in path
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
	checks various built-in commands and execute the corresponding functions or return an exit code.
	checking what builti function returns if false return ret value
	else if builti returns true, then it checks the argv[0] if it's "env", call the env_cmd func
	else if the argv[0] starts with "./", "../", or "/", indicating that it's a command to execute
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
	handling the "cd" (change directory) command
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
		//if (data->argc == 1)  UNIMPLEMENTED
		//	return ;
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
	it means that input redirection is specified in the command line.
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
	uses fork-exec-wait to run the external program in a separate
	child process, and provides an exit status based on the success
	or failure of the execution

	make a copy of command list
	handle SIGINT and handle keyboard interrupts in the child proccess
	
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
