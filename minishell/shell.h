/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:38:48 by achak             #+#    #+#             */
/*   Updated: 2024/04/22 15:48:00 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <termios.h>
# include <curses.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "get_next_line.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command
{
	char	**cmd_args;
	char	*cmd_path;
	char	*raw_str;
	char	*heredoc;
	int		stdin_fd;
	int		stdout_fd;
	int		heredoc_rightmost;
}	t_command;

typedef struct s_params
{
	char		*line_read;
	t_command	*cmd_arr;
	pid_t		*pid_arr;
	int			cmd_nbr;
	t_env		**head_env;
}	t_params;

//static volatile sig_atomic_t	g_async_flag = 0;

extern int	g_async_flag;

//	main.c
int		split_raw_str_by_pipes(t_params *params, char *line_read);
int		sigint_detected(t_params *params);
void	main_shell_loop(t_params *params, char *line_read);

//	count_len_til_pipe.c
void	copy_chars_in_quoted_str(t_params *params, int i, char **temp, int *j);
int		copy_line_til_pipe2(char *raw_str, int *j, char **temp,
			int *flag);
void	copy_line_til_pipe(t_params *params, int i, char **temp);

//	copy_len_til_pipe.c
void	count_chars_in_quoted_str(char **temp, int *count);
int		count_len_til_pipe2(char **temp, int *flag, int *count);
int		count_len_til_pipe(char *temp);

//	signal_handlers.c
void	signal_handler(int sig);
void	set_up_signals(void);
void	set_signals_to_dfl(void);
void	non_interactive_signal_handler(void);

//	misc_funcs_for_main.c
void	init_cmd_arr(t_params *params, t_command *cmd_arr, int cmd_nbr,
			char *line_read);
void	init_params(t_params *params, t_env **head_env);
void	check_read_line_eof(char *line_read, int flag, t_params *params);

//	settle_child_process_stdin_and_out.c
void	settle_child_process_stdin2(t_params *params, int i);
void	settle_child_process_stdin3(t_params *params, int i, int *old_fd);
void	settle_child_process_stdin(t_params *params, int i, int *new_fd,
			int *old_fd);
void	settle_child_process_stdout2(t_params *params, int i, int *new_fd);
void	settle_child_process_stdout(t_params *params, int i, int *new_fd,
			int *old_fd);

//	child_process.c
void	cleanup_in_child_process(t_params *params, char **envp, int *new_fds,
			int *old_fds);
void	call_execve(t_params *params, int i, int *new_fds, int *old_fds);
int		child_process2(t_params *params, int i, int *new_fds, int *old_fds);
int		child_process1(t_params *params, int i, int *new_fds, int *old_fds);
int		child_process(t_params *params, int i, int *new_fds, int *old_fds);

//	copy_str_to_filename.c
t_env	*is_valid_var(t_env *temp_env, char *temp, int j);
int		copy_valid_var(t_env *temp_env, char *filename, int *k);
void	copy_var_len_redir(t_params *params, char **temp, int *k,
			char *filename);
int		copy_str_to_filename2(char *filename, char *temp, int *flag, int *k);
void	copy_str_to_filename(t_params *params, char *filename, char *temp);

//	expand_filename_and_count_word_len.c
int		count_valid_var(t_env *temp_env, int *word_len);
void	count_var_len_redir(t_params *params, char **temp, int *word_len);
int		count_word_len2(char *temp, int *word_len, int *k);
int		count_word_len(t_params *params, char *temp);
char	*expand_filename(t_params *params, char *temp);

//	open_redirects2.c
int		check_type_of_redirect(char *temp);
void	close_existing_fds(t_params *params, int i, int open_mode);
int		open_call_error_or_not(t_params *params, int i, char *filename);
void	erase_redirs_from_str(char **temp, int *flag);

//	open_redirects.c
int		perror_filename(char *filename, int open_mode);
int		open_new_files(t_params *params, int i, int open_mode,
			char *filename);
int		settle_redirects(t_params *params, int i, char **temp);
void	skip_quoted_chars(char *temp, int *flag);
int		open_redirects(t_params *params, int i);

//	cd_builtin2.c
int		free_cd_strings(char *oldpwd, char **dir_arr, int flag);
char	*find_env_var_value(t_env *head_env, char *var);
int		check_valid_dir(t_env **head_env, char *dir_arr, char **new_path,
			char *path);
int		check_present_dir(t_env **head_env, char *path);
int		search_dir_arr(t_env **head_env, char **dir_arr, char *path);

//	cd_builtin.c
//int		free_cd_strings(har **dir_arr, int flag);
int		check_dir_path_has_c(char *dir_path, char c);
void	cd_wrapper2(t_env **head_env, char *oldpwd, char *ch_path);
int		cd_wrapper(t_env **head_env, char *new_path, char *path,
			char **dir_arr);
int		different_cd_criteria(t_env **head_env, char **cmd_args, char *cd_path,
			char **dir_arr);
int		cd_builtin(t_env **head_env, char **cmd_args);

//	implement_builtins.c
void	exit_builtin(t_params *params, char **cmd_args);
int		echo_builtin(char **cmd_args);
int		pwd_builtin(t_env *head_env);
int		env_builtin(t_env *head_env);
int		unset_builtin(t_env **head_env, char **cmd_args);

//	export_builtin.c
int		check_if_entry_exists(char *var, t_env *head_env);
void	update_existing_entry(char *var, t_env **head_env);
int		check_valid_var_name(char *var);
int		export_var_into_env_list(t_env **head_env, char **cmd_args);
int		export_builtin(t_env **head_env, char **cmd_args);

//	execute_builtin.c
void	preserve_stdin_and_stdout(t_params *params, int i, int *dup_stdin,
			int *dup_stdout);
void	restore_stdin_and_stdout(int dup_stdin, int dup_stdout);
void	choose_which_builtin_to_exec(t_params *params, int i, int *exit_status);
int		execute_builtin(t_params *params, int i, int flag, int *old_fds);

//	check_if_first_cmd_is_builtin.c
int		check_if_cmd_is_builtin(char *cmd);
int		check_if_fork_needed(t_params *params, char *line_read);
void	skip_redir_and_filename(char **line_read);
int		determine_whats_first_word(char *line_read, t_env *head_env);
int		check_if_first_cmd_is_builtin(char *line_read, t_env *head_env);

//	fork_and_execve.c
void	wrapper(int func_rv, const char *func_name);
void	fork_and_execve(t_params *params, int i, int *new_fds, int *old_fds);
void	set_exit_status(t_params *params, int wstatus, int mode);
void	parent_waits_for_children(t_params *params, int *old_fds);
void	preparing_fork_and_execve(t_params *params, char *line_read);

//	locate_command_names.c
int		check_for_char_in_str(char *str, char c);
int		check_and_assign_cmd_path2(t_params *params, int i);
int		check_and_assign_cmd_path(t_params *params, int i, char **arr_path);
int		check_if_path_var_exists(t_env *temp, t_params *params, int i,
			char ***arr_path);
int		locate_command_names(t_params *params, int i);

//	copy_token_from_line2.c
void	copy_meta_char(char **temp, char *token_arr, int *i);
int		copy_quote_len(char **temp, int *i, char *token_arr, t_env *head_env);

//	copy_token_from_line.c
int		move_ptr_past_var(char **temp, t_env **head_env);
void	dont_copy_if_invalid_var(char **temp, char *token_arr, int j,
			int *i);
void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env);
void	copy_regular_len(char **temp, char *token_arr, int *i);
void	copy_token_from_line(char **temp, char *token_arr, t_env *head_env);

//	count_token_len.c
void	count_var_len2(char **temp, int *token_len, t_env *head_env, int j);
void	dont_count_if_invalid_var(char **temp, int j, int *token_len);
void	count_var_len(char **temp, int *token_len, t_env *head_env, int flag);
void	count_regular_len(char **temp, int *token_len);

//	parse_input_string.c
void	count_meta_char(char **temp_line, int *token_len);
void	count_quote_len(char **temp_line, char quote, int *token_len,
			t_env *head_env);
int		count_token_len(char *temp, t_env *head_env);
//void	check_if_heredoc_prev_token(char **token);
void	alloc_token_by_index(char **temp_line, char **token_arr, int i,
			t_env *head_env);
char	**parse_input_string(char *line_read, t_env *head_env);

//	count_nbr_of_tokens.c
void	iterate_thru_quotes(char **temp, char quote, int *count, int *flag);
void	check_if_valid_var2(char **temp, int *i);
void	check_if_valid_var(char **temp, t_env *head_env, int *count, int *flag);
int		count_nbr_of_tokens(char *line_read, t_env *head_env);

//	count_nbr_of_tokens2.c
void	iterate_thru_meta_char(char **temp, int *count, int *flag);
void	iterate_thru_word(char **temp, int *count, int *flag);
void	reset_flag_for_new_word(char **line_read, int *flag);

//	nbr_of_pipes.c
void	skip_til_end_of_quotes(char **line_read);
int		count_if_valid_pipe(char **line_read, t_env **head_env, int *flag,
			int *nbr);
int		nbr_of_pipes(char *line_read, t_env **head_env);
int		check_empty_line(char *line_read);

//	check_syntax.c
void	update_syntax_error_exit_code(t_env **head_env);
int		check_after_meta_char2(char **line_read);
int		check_after_meta_char(char **line_read);
int		check_redir_syntax(char *line_read, t_env **head_env);
int		check_syntax_around_pipe(char **line_read);

//	alloc_new_heredoc_copy.c
int		look_for_env_var(char *str);
void	copy_heredoc_for_var(char **str, char *here_doc, int *i, t_env *temp);
void	copy_new_heredoc(char *str, char *here_doc, t_params *params);

//	alloc_new_heredoc_count.c
char	*alloc_new_heredoc(char *str, t_params *params);
void	count_heredoclen_for_var(char **str, int *count, t_params *params);
int		count_new_heredoc_len(char *str, t_params *params);

//	count_delim_len.c
int		check_quotes_in_heredoc_delim(char *line_read);
void	go_thru_quotes_for_delim(char **line_read, int *delim_len, int *flag);
int		count_delim_len_redir(char **line_read, int *delim_len, int *flag);
int		count_delim_len(char *line_read);

//	alloc_delim_str.c
void	copy_quoted_parts_in_delim(char **line_read, char *delim, int *flag,
			int *i);
int		copy_delim_str_redir(char **line_read, char *delim, int *flag,
			int *i);
void	copy_delim_str(char *line_read, char *delim, int flag, int i);
char	*alloc_delim_str(char *line_read, int delim_len);

//	heredoc_signals.c
void	heredoc_sighandler(int sig);
void	heredoc_signal_handler(void);
void	free_heredoc_stuff(char *str, char *heredoc, char *delim, int fd1);
int		cleanup_before_exiting_heredoc(t_params *params, int i);

//	ft_heredoc.c
int		save_heredoc_to_struct(t_command *cmd_arr, int fd1, int fd2,
			char *heredoc);
int		check_heredoc_eof(char *str, char *delim);
int		ft_heredoc(t_params *params, int i, char *delim, int flag);
void	ignore_quotes_finding_other_heredocs(char **line_read, int *flag);
int		is_heredoc_rightmost(char *line_read, int delim_len);

//	open_all_heredocs.c
void	erase_quoted_portion_in_heredoc(char **line_read, int *flag);
void	erase_heredoc_and_delim(char **line_read, int *flag);
//void	erase_heredoc_and_delim(char **line_read, int *flag);
int		check_for_heredoc(t_params *params, int i, char **line_read);
void	ignore_quotes_looking_for_heredoc(char **line_read, int *flag);
int		open_all_heredocs(t_params *params, char *line_read);

//	create_symbol_table.c
char	*copy_value_from_var(char *value, char *var, int i);
char	*get_value_from_var(char *var);
void	create_new_entry(char *var, t_env **head);
t_env	*create_symbol_table(int ac, char **av, char **env);

//	symbol_table_funcs.c
void	remove_head_ptr_or_not(t_env **head, t_env *remove_node,
			t_env *prev_node, t_env *next_node);
void	remove_table_entry(t_env **head, char *key);
char	*get_key_from_var(char *var);

//	create_envp_arr.c
int		count_env_nbr(t_env *head_env);
int		copy_strs_to_envp_arr(int i, char **envp, t_env *temp);
char	**create_envp_arr(t_env *head_env);

//	helper_fns2.c
void	ft_memset(void *s, int c, size_t n);
int		my_strcmp(char *delimiter, char *str);
int		is_alphabet(char c);
int		is_numeric(char c);

//	helper_fns.c
int		my_strlen(char *s);
int		my_strncmp(char *s1, char *s2, size_t n);
void	free_str_nbr(char *s1, char *s2, int str_nbr);
char	*strjoin_and_free_str(char *s1, char *s2, int str_nbr);
int		is_whitespace(char c);

//	cleanup_fns.c
void	free_symbol_table(t_env **head);
void	free_array(char **arr);
void	free_cmd_arr(t_command *cmd_arr, int cmd_nbr);
void	cleanup_params(t_params *params);

int		ft_dprintf(int fd, const char *format, ...);
int		print_char(int c, int fd);
int		print_str(char *str, int fd);
int		print_digit(long n, int base, char specifier, int fd);
int		print_format(char sp, va_list ap, int fd);

int		ft_ptrlen(unsigned long int ptr);
void	ft_putptr(unsigned long int ptr, int fd);
int		print_ptr(unsigned long int ptr, int fd);

char	*ft_itoa(int n);

char	**ft_split(char *str, char sep);

int		ft_atoi(char *nptr);

#endif
