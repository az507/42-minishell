/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:38:48 by achak             #+#    #+#             */
/*   Updated: 2024/04/07 17:54:07 by achak            ###   ########.fr       */
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
	char		*key;
	char		*value;
	struct s_env	*next;
}	t_env;

typedef struct s_command
{
	char	**cmd_args;
	//char	**filenames;
	char	*raw_str;
	char	*heredoc;
	int		stdin_fd;
	int		stdout_fd;
	//int		heredoc_fd;
	int		heredoc_rightmost;
}	t_command;

typedef struct s_params
{
	t_command	*cmd_arr;
	//int		heredoc_count;
	int		cmd_nbr;
	t_env	**head_env;
}	t_params;

//	main.c
int		count_len_til_pipe(char *temp);
void	copy_line_til_pipe(t_params *params, int i, char **temp);
void	split_raw_str_by_pipes(t_params *params, char *line_read);
void	main_shell_loop(t_params *params, char *line_read);

//	misc_funcs_for_main.c
void	signal_handler(int sig);
void	set_up_signals(void);
void	init_cmd_arr(t_command *cmd_arr, int cmd_nbr);
void	init_params(t_params *params, t_env **head_env);
void	check_read_line_eof(char *line_read, int flag, t_params *params);

//	settle_child_process_stdin_and_out.c
void	settle_child_process_stdin2(t_params *params, int i);
void	settle_child_process_stdin(t_params *params, int i, int *old_fds);
void	settle_child_process_stdout(t_params *params, int i, int *new_fds);

//	child_process.c
int		check_type_of_redirect(char *temp);
int		count_valid_var(t_env *temp_env, int *word_len);
void	count_var_len_redir(t_params *params, char **temp, int *word_len);
int		count_word_len2(char *temp, int *word_len, int *k);
int		count_word_len(t_params *params, char *temp);
t_env	*is_valid_var(t_env *temp_env, char *temp, int j);
int		copy_valid_var(t_env *temp_env, char *filename, int *k);
void	copy_var_len_redir(t_params *params, char **temp, int *k, char *filename);
int		copy_str_to_filename2(char *filename, char *temp, int *flag, int *k);
void	copy_str_to_filename(t_params *params, char *filename, char *temp);
char	*expand_filename(t_params *params, char *temp);
void	close_existing_fds(t_params *params, int i, int open_mode);
int		open_call_error_or_not(t_params *params, int i, char *filename);
int		open_new_files(t_params *params, int i, int open_mode, char *filename);
void	erase_heredoc_and_delim(char **temp, int *flag);
int		perror_filename(char *filename, int open_mode);
int		settle_redirects(t_params *params, int i, char **temp);
void	skip_quoted_chars(char *temp, int *flag);
int		open_redirects(t_params *params, int i);
void	child_process(t_params *params, int i, int *new_fds, int *old_fds);

//	cd_builtin2.c
char	*find_env_var_value(t_env *head_env, char *var);
int		check_valid_dir(t_env **head_env, char *dir_arr, char **new_path,
			char *path);
int		check_present_dir(t_env **head_env, char *path);
int		search_dir_arr(t_env **head_env, char **dir_arr, char *path);

//	cd_builtin.c
int		free_cd_strings(char *oldpwd, char *pwd, char **dir_arr, int flag);
//int		free_cd_strings(har **dir_arr, int flag);
int		check_dir_path_has_c(char *dir_path, char c);
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

//	fork_and_execve.c
void	wrapper(int func_rv, const char *func_name);
int		check_if_cmd_is_builtin(char *cmd);
char	*check_if_fork_neededd(t_params *params, char *line_read);
void	skip_redir_and_filename(char **line_read);
char	*determine_whats_first_word(char *line_read, t_env *head_env);
char	*check_if_first_cmd_is_builtin(char *line_read, t_env *head_env);
void	fork_and_execve(t_params *params, int i, int *new_fds, int *old_fds);
void	preparing_fork_and_execve(t_params *params, char *line_read);

//	locate_command_names.c
int		check_for_char_in_str(char *str, char c);
int		check_and_assign_cmd_path2(t_params *params, int i);
int		check_and_assign_cmd_path(t_params *params, int i, char **arr_path);
int		locate_command_names(t_params *params, int i);

//	copy_token_from_line2.c
void	copy_meta_char(char **temp, char *token_arr, int *i);
int		copy_quote_len(char **temp, int *i, char *token_arr, t_env *head_env);
int		move_ptr_past_var(char **temp, t_env **head_env);

//	copy_token_from_line.c
void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env);
void	copy_regular_len(char **temp, char *token_arr, int *i);
int		copy_token_from_line(char **temp, char *token_arr, t_env *head_env);

//	count_token_len.c
void	count_var_len2(char **temp, int *token_len, t_env *head_env, int j);
void	count_var_len(char **temp, int *token_len, t_env *head_env);
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
void	check_if_valid_var(char **temp, t_env *head_env, int *count, int *flag);
int		count_nbr_of_tokens(char *line_read, t_env *head_env);

//	count_nbr_of_tokens2.c
void	iterate_thru_meta_char(char **temp, int *count, int *flag);
void	iterate_thru_word(char **temp, int *count, int *flag);
void	reset_flag_for_new_word(char **line_read, int *flag);

//	check_syntax.c
int		check_after_meta_char(char **line_read);
int		check_redir_syntax(char *line_read);
int		check_syntax_around_pipe(char **line_read);
int		nbr_of_pipes(char *line_read);
int		check_empty_line(char *line_read);

//	alloc_new_heredoc.c
int		look_for_env_var(char *str);
void	copy_new_heredoc(char *str, char *here_doc, t_params *params);
char	*alloc_new_heredoc(char *str, t_params *params);
int		count_new_heredoc_len(char *str, t_params *params);

//	open_all_heredocs.c
int		check_quotes_in_heredoc_delim(char *line_read);
int		count_delim_len(char *line_read);
char	*alloc_delim_str(char *line_read, int delim_len);
void	ft_heredoc(t_params *params, int i, char *delim, int flag);
int		is_heredoc_rightmost(char *line_read, int delim_len);
void	check_for_heredoc(t_params *params, int i, char **line_read);
void	open_all_heredocs(t_params *params, char *line_read);

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