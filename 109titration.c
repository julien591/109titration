/*
** C:\Users\jlele\Desktop\rendu\Maths\109titration\109titration.c for 109titration in
**
** Made by Julien Leleu
** Login   <leleu_j@epitech.eu>
**
** Started on  Tue Mar 22 18:49:20 2016 Julien Leleu
** Last update Wed Apr  6 12:04:28 2016 John Doe
*/

#define	 _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int	check_arg(int ac, char **av)
{
  int	fd;

  if (ac != 2)
    {
      fputs("Error, enter one file\n", stderr);
      exit (84);
      return (0);
    }
  if (strcmp(av[1], "-h") == 0)
    {
      printf("USAGE\n\t./109titration file\n\nDESCRIPTION\n\tfile\ta csv file containing 'vol;ph' lines\n");
      exit (0);
      return (0);
    }
  /*if (strlen(av[1]) < 4)
    {
      fputs("Error: file must be name.csv\n", stderr);
      exit (84);
      return (0);
    }
  if (strcmp(".csv", &av[1][strlen(av[1]) - 4]) != 0)
    {
      fputs("Error: not a CSV file\n", stderr);
      exit (84);
      return (0);
    }*/
  if ((fd = open(av[1], O_RDONLY)) == -1)
    {
      fputs("Error: cannot open your file or it does not exist\n", stderr);
      exit (84);
      return (0);
    }
  close(fd);
  return (0);
}

int	how_line(char *str)
{
  int	line = 0;
  FILE*	file;
  char buf[256];

  file = fopen(str, "r");
  while (fgets(buf, sizeof(buf), file))
    line++;
  fclose(file);
  return(line);
}

int	is_empty(char *str)
{
  FILE *file;

  file = fopen(str, "r");
  fseek(file, 0, SEEK_END);
  if (ftell(file) == 0)
    {
      fputs("Error, your file is empty\n", stderr);
      exit (84);
      return (0);
    }
  fclose(file);
  return (0);
}

char	**create_tmp(char *str, int line)
{
  FILE*	file;
  char	**data;
  char	*buf;
  size_t size;
  int	i = 0;
  int	j;

  is_empty(str);
  file = fopen(str, "r");
  buf = NULL;
  data = malloc(sizeof(char *) * (line + 1));
  if (data == NULL)
    return (NULL);
  while (i < line)
    {
      j = getline(&buf, &size, file);
      if (j == -1)
	return (NULL);
      data[i] = malloc(sizeof(char) * (strlen(buf) + 1));
      if (data[i] == NULL)
	return (NULL);
      strcpy(data[i], buf);
      data[i][strlen(buf) - 1] = '\0';
      i++;
    }
  free (buf);
  data[i] = NULL;
  fclose(file);
  return (data);
}

void	super_free(char **tmp, double **data, double **deriv, double **second, double **final, int line, double how)
{
  int	i = 0;

  while (tmp[i] != '\0')
    {
      free(tmp[i]);
      i++;
    }
  free(tmp);
  i = 0;
  while (i < line)
    {
      free(data[i]);
      i++;
    }
  free(data);
  i = 0;
  while (i < line - 2)
    {
      free(deriv[i]);
      i++;
    }
  free(deriv);
  i = 0;
  while (i < line - 4)
    {
      free(second[i]);
      i++;
    }
  free(second);
  i = 0;
  while (i != how)
    {
      free(final[i]);
      i++;
    }
  free(final);
}

int	check_values(char **tmp)
{
  int	i = 0;
  int	j;

  while (tmp[i] != NULL)
    {
      j = 0;
      while (tmp[i][j] != '\0' && tmp[i][j] != '\n')
	{
	  if ((tmp[i][j] >= '0' && tmp[i][j] <= '9') || tmp[i][j] == ';' || tmp[i][j] == '.' || tmp[i][j] == '-')
	    j++;
	  else
	    {
	      fputs("There is an error in your file, please check\n", stderr);
	      exit (84);
	      return (0);
	    }
	}
      i++;
    }
  return (0);
}

double	**create_data(char **tmp, int line)
{
  char	*cpy;
  char	*delim = ";";
  double **data;
  int	i = 0;;

  data = malloc(sizeof(double *) * (line + 1));
  if (data == NULL)
    return (NULL);
  while (i < line)
    {
      data[i] = malloc(sizeof(double) * 2);
      if (data[i] == NULL)
	return (NULL);
      cpy = tmp[i];
      data[i][0] = atof(strsep(&cpy, delim));
      data[i][1] = atof(strsep(&cpy, delim));
      i++;
    }
  free(cpy);
  return (data);
}

double	**derivative(double **data, int line)
{
  double **derivative;
  int	i = 0;
  int	j = 1;

  derivative = malloc(sizeof(double *) * (line - 1));
  if (derivative == NULL)
    return (NULL);
  while (i < line - 2)
    {
      derivative[i] = malloc(sizeof(double) * 2);
      if (derivative[i] == NULL)
	return (NULL);
      derivative[i][0] = data[j][0];
      derivative[i][1] = (data[j + 1][1] - data[j - 1][1]) / (data[j + 1][0] - data[j - 1][0]);
      i++;
      j++;
    }
  return (derivative);
}

double **second_derivative(double **deriv, int line)
{
  double **second_derivative;
  int	i = 0;
  int	j = 1;

  second_derivative = malloc(sizeof(double *) * (line - 3));
  if (second_derivative == NULL)
    return (NULL);
  while (i < line - 4)
    {
      second_derivative[i] = malloc(sizeof(double) * 2);
      if (second_derivative[i] == NULL)
	return (NULL);
      second_derivative[i][0] = deriv[j][0];
      second_derivative[i][1] = (deriv[j + 1][1] - deriv[j - 1][1]) / (deriv[j + 1][0] - deriv[j - 1][0]);
      i++;
      j++;
    }
  return (second_derivative);
}

double	**final_derivative(double **second, double how, int case_equi)
{
  double **final;
  int i = 0;
  double pas = 0;
  double j;
  double first_arg = second[case_equi - 1][0];

  final = malloc(sizeof(double *) * how);
  if (final == NULL)
    return (NULL);
  while (i != how)
    {
      final[i] = malloc(sizeof(double) * 2);
      if (final[i] == NULL)
	return (NULL);
      final[i][0] = first_arg;
      i++;
      first_arg = first_arg + 0.1;
    }
  i = 1;
  pas = (second[case_equi][1] - second[case_equi - 1][1]) / ((second[case_equi][0] - second[case_equi - 1][0]) * 10);
  final[0][1] = second[case_equi - 1][1];
  j = final[0][0];
  while (j < second[case_equi][0] - 0.1)
    {
      final[i][1] = final[i - 1][1] + pas;
      j = j + 0.1;
      i++;
    }
  pas = (second[case_equi + 1][1] - second[case_equi][1]) / ((second[case_equi + 1][0] - second[case_equi][0]) * 10);
  while (j < second[case_equi + 1][0] - 0.1)
    {
      final[i][1] = final[i - 1][1] + pas;
      j = j + 0.1;
      i++;
    }
  return (final);
}

double	max_first(double **first_deriv, int line)
{
  int	i = 1;
  int	j;
  double max_result = first_deriv[0][1];

  while (i < line - 2)
    {
      if (first_deriv[i][1] > max_result)
	{
	  j = i;
	  max_result = first_deriv[j][1];
	}
      i++;
    }
  return (first_deriv[j][0]);
}

double	exact_equivalent(double **final, int limit)
{
  int	i = 0;

  while (i != limit)
    {
      if (final[i][1] * final[i + 1][1] < 0)
	return (final[i][0]);
      i++;
    }
  return (0);
}

int	main(int ac, char **av)
{
  char	**tmp;
  double **data;
  double **first_deriv;
  double **second_deriv;
  double **final;
  int	line;
  double how;
  double equi;
  int i = 0;

  check_arg(ac, av);
  line = how_line(av[1]);
  tmp = create_tmp(av[1], line);
  check_values(tmp);
  data = create_data(tmp, line);
  first_deriv = derivative(data, line);
  puts("Derivative:");
  while (i < line - 2)
    {
      printf("volume: %g ml -> %.2f\n", first_deriv[i][0], first_deriv[i][1]);
      i++;
    }
  equi = max_first(first_deriv, line);
  printf("\nEquivalent point at %g ml\n\n", equi);
  puts("Second derivative:");
  second_deriv = second_derivative(first_deriv, line);
  i = 0;
  while (i < line - 4)
    {
      printf("volume: %g ml -> %.2f\n", second_deriv[i][0], second_deriv[i][1]);
      i++;
    }
  puts("\nSecond derivative estimated:");
  i = 0;
  while (second_deriv[i][0] != equi)
    i++;
  how = (second_deriv[i + 1][0] - second_deriv[i - 1][0]) * 10 + 1;
  final = final_derivative(second_deriv, how, i);
  i = 0;
  while (i != how)
    {
      printf("volume: %g ml -> %.2f\n", final[i][0], final[i][1]);
      i++;
    }
  equi = exact_equivalent(final, how);
  printf("\nEquivalent point at %g ml\n", equi);
  super_free(tmp, data, first_deriv, second_deriv, final, line, how);
  return (0);
}
