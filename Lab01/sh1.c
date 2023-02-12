// Shell.

#include "types.h"
#include "user.h"
#include "fcntl.h"

// Parsed command representation
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

#define MAXARGS 10

struct cmd {
  int type;
};

struct execcmd {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};

struct redircmd {
  int type;
  struct cmd *cmd;
  char *file;
  char *efile;
  int mode;
  int fd;
};

struct pipecmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct listcmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct backcmd {
  int type;
  struct cmd *cmd;
};

int fork1(void);  // Fork but panics on failure.
void panic(char*);
struct cmd *parsecmd(char*);

// Execute cmd.  Never returns.
void
runcmd(struct cmd *cmd)
{
  // printf(1, "runcmd has started execution \n");
  int p[2];
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    exit(0);

  switch(cmd->type){
  default:
    panic("runcmd");

  // nonohup implemented inside EXEC command to make it easier to look up 
  case EXEC:
    // printf(2, "runcmd EXEC has started execution \n");
    ecmd = (struct execcmd*)cmd;
    if(ecmd->argv[0] == 0){
      exit();
    }
    // checks if the first argument of the execute funciton is nonohup
    else if(strcmp(ecmd->argv[0],"nonohup")==0){
      // if its nonohup, it performs the functionality similar to & where it creates a fork and runs the
      // from the forked child process thereby running it in the background.
      if (strcmp(ecmd->argv[1], "alsoNice") == 0){
        int clocktick = atoi(ecmd->argv[2]);
        if (alsoNice(clocktick) == 0){
            // printf(1, "Fork is successful");
            exec(ecmd->argv[3], &(ecmd->argv[3]));
        } else{
          wait();
        }
      }
      else if(fork1() == 0){
        exec(ecmd->argv[1], &(ecmd->argv[1]));
      }
    }
    else if (strcmp(ecmd->argv[0], "alsoNice") == 0){
      int clocktick = atoi(ecmd->argv[1]);
      if (clocktick < 0){
        printf(2, "Required: clocktick > 0 ");
      }
      int fid = alsoNice(clocktick);
      if (fid == 0){
          exec(ecmd->argv[2], &(ecmd->argv[2]));
      } else{
        wait();
      }
    }
    // every other argument will run in a regular way
    else{
      exec(ecmd->argv[0], ecmd->argv);
      // the print is only executed when the previous statement fails
      printf(2, "exec %s failed\n", ecmd->argv[0]);
    }
    break;

  case ' ':
    ecmd = (struct execcmd*)cmd;

    printf("These are the arguments in argv[]: ");

    for (int i = 0; i < 10; i++){
      if (ecmd->argv[i] != NULL){
        printf("%d ", i);
        printf("%s ", ecmd->argv[i]);
      }
    }
    printf("\n");

    if(ecmd->argv[0] == 0){ //If this is nothing exit
      exit(0);
    }

    // checks if the first argument of the execute funciton is nonohup
    else if(strcmp(ecmd->argv[0],"nonohup")==0){

      if(fork1() == 0){
        execvp(ecmd->argv[1], &(ecmd->argv[1]));
      }
    }
    else{
      execl(ecmd->argv[0], ecmd->argv);
      // the print is only executed when the previous statement fails
      printf("exec %s failed\n", ecmd->argv[0]);
    }
    exit(0);
    break;

  case ' ':

    ecmd = (struct execcmd*)cmd;
    
    //This will test and display the arguments for the command to be executed that was captured from the user
    printf("These are the arguments in argv[]: ");

    for (int i = 0; i < 10; i++){
      if (ecmd->argv[i] != NULL){
        printf("%d ", i);
        printf("%s ", ecmd->argv[i]);
      }
    }
    printf("\n");


    //check if there is a command coming through if not kill the input
    if(ecmd->argv[0] == 0){
      exit(0);
    }

    //execvp will be used here because the first argument requests a program name, then the second argument is a array of the arguments for the program we intend to execute.
    execvp(ecmd->argv[0], ecmd->argv);
    break;

  case ' ':
    ecmd = (struct execcmd*)cmd;

    printf("These are the arguments in argv[]: ");

    for (int i = 0; i < 10; i++){
      if (ecmd->argv[i] != NULL){
        printf("%d ", i);
        printf("%s ", ecmd->argv[i]);
      }
    }
    printf("\n");

    if(ecmd->argv[0] == 0){ //If this is nothing exit
      exit(0);
    }
    if(strcmp(ecmd->argv[0],"nonohup")==0){
      printf("this command is nonohup \n");
      if(fork1() == 0){
        printf("fork is running");
        execvp(ecmd->argv[1], &(ecmd->argv[1]));
      }
    }
    else{
      printf("THIS COMMAND IS NOT NONOHUP \n");
      execvp(ecmd->argv[0], &(ecmd->argv[0]));
      
      // the print is only executed when the previous statement fails
      printf("Command %s Failed / Not found\n", ecmd->argv[0]);
    }
    exit(-1);
    printf("fork is over");
    break;


  case ' ':
    ecmd = (struct execcmd*)cmd;

    //this prints the args so i know
    printf("These are the arguments in argv[]: ");

    for (int i = 0; i < 10; i++){
      if (ecmd->argv[i] != NULL){
        printf("%d ", i);
        printf("%s ", ecmd->argv[i]);
      }
    }
    printf("\n");

    //this checks if their is a command to run
    if(ecmd->argv[0] == 0){ //If this is nothing exit
      exit(0);
    }

    char nonoarg[]="nonohup";
    char* n = ecmd->argv[0]; //this would be the first char str detected
    
    for (int x=0; *n!=0; x++){
      if (*n!=nonoarg[x]){
        break;
      }
      n++;
    }
    if (*n == 0){ //this will actually do a nonohup command
      int argc=0;
      for(; argc<MAXARGS-1;argc++){
        if (ecmd->argv[argc] == 0){
          break; //no arg cause it points to nothing
        }
        else{
          ecmd->argv[argc]=ecmd->argv[argc+1];
        }
      }
      ecmd->argv[argc]=0;

      if (ecmd->argv[0] == 0){
        printf("This is a nonohup command not being folloed by a command! \n");
        exit(0);
      }

      if(fork1() == 0){ //create a new child and leave it with no waiting
        close(1);

        runcmd(cmd);

        exit(0);

      }
      else{

        execvp(ecmd->argv[0], ecmd->argv);
        printf("Command %s Failed \n", ecmd->argv[0]);
      }
    } 

  case REDIR:
    // printf(1, "runcmd REDIR has started execution \n");
    rcmd = (struct redircmd*)cmd;
    close(rcmd->fd);
    if(open(rcmd->file, rcmd->mode) < 0){
      printf(2, "open %s failed\n", rcmd->file);
      exit();
    }
    runcmd(rcmd->cmd);
    break;

  case LIST:
    // printf(2, "runcmd LIST has started execution \n");
    lcmd = (struct listcmd*)cmd;
    if(fork1() == 0)
      runcmd(lcmd->left);
    wait();
    runcmd(lcmd->right);
    break;

  case PIPE:
    // printf(1, "runcmd PIPE has started execution \n");
    pcmd = (struct pipecmd*)cmd;
    if(pipe(p) < 0)
      panic("pipe");
    if(fork1() == 0){
      // printf(1,"Pipe has been called on the left command");
      close(1);
      dup(p[1]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->left);
    }
    if(fork1() == 0){
      // printf(2,"Pipe has been called on the right command");
      close(0);
      dup(p[0]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->right);
    }
    close(p[0]);
    close(p[1]);
    wait();
    wait();
    break;

  case BACK:
    // printf(2, "runcmd BACK has started execution \n");
    bcmd = (struct backcmd*)cmd;
    if(fork1() == 0)
      runcmd(bcmd->cmd);
    break;
  }
  exit();
}

int
getcmd(char *buf, int nbuf)
{
  // printf(1, "getcmd has started \n");
  printf(2, "$S20 ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(void)
{
  static char buf[100];
  int fd;

  // Ensure that three file descriptors are open.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  // printf(2, "main started and calling getcmd \n");
  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Chdir must be called by the parent, not the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        printf(2, "cannot cd %s\n", buf+3);
      continue;
    }
    if(fork1() == 0){
      // printf(2,"A child process has been created \n");
      runcmd(parsecmd(buf));
    }
    wait();
  }
  exit();
}

void
panic(char *s)
{
  printf(2, "%s\n", s);
  exit();
}

int
fork1(void)
{
  // printf(1, "fork1 is called \n");
  int pid;
  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

//PAGEBREAK!
// Constructors

struct cmd*
execcmd(void)
{
  // printf(1, "execcmd has been executed \n");
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd*
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  // printf(1, "redircmd has been executed \n");
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return (struct cmd*)cmd;
}

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
  // printf(1, "pipecmd has been executed \n");
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd*
listcmd(struct cmd *left, struct cmd *right)
{
  // printf(1, "listcmd has been executed \n");
  struct listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = LIST;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd*
backcmd(struct cmd *subcmd)
{
  // printf(1, "backcmd has been executed \n");
  struct backcmd *cmd;
  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = BACK;
  cmd->cmd = subcmd;
  return (struct cmd*)cmd;
}
//PAGEBREAK!
// Parsing

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int
gettoken(char **ps, char *es, char **q, char **eq)
{
  // printf(1,"gettoken has been called \n");
  char *s;
  int ret;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  if(q)
    *q = s;
  ret = *s;
  switch(*s){
    case 0:
      break;
    case '|':
    case '(':
    case ')':
    case ';':
    case '&':
    case '<':
      s++;
      break;
    case '>':
      s++;
      if(*s == '>'){
        ret = '+';
        s++;
    }
    break;
  default:
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if(eq)
    *eq = s;

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int
peek(char **ps, char *es, char *toks)
{
  // printf(1,"peek has been executed \n");
  char *s;
  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *nulterminate(struct cmd*);

struct cmd*
parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;
  es = s + strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if(s != es){
    printf(2, "leftovers: %s\n", s);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

struct cmd*
parseline(char **ps, char *es)
{
  struct cmd *cmd;
  // printf(2, "es -> %s\n", es);
  // printf(2, "ps -> %s\n", es);
  cmd = parsepipe(ps, es);
  while(peek(ps, es, "&")){
    gettoken(ps, es, 0, 0);
    cmd = backcmd(cmd);
  }
  if(peek(ps, es, ";")){
    gettoken(ps, es, 0, 0);
    cmd = listcmd(cmd, parseline(ps, es));
  }
  return cmd;
}

struct cmd*
parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd*
parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;
  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
      panic("missing file for redirection");
    switch(tok){
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    case '+':  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    }
  }
  return cmd;
}

struct cmd*
parseblock(char **ps, char *es)
{
  struct cmd *cmd;

  if(!peek(ps, es, "("))
    panic("parseblock");
  gettoken(ps, es, 0, 0);
  cmd = parseline(ps, es);
  if(!peek(ps, es, ")"))
    panic("syntax - missing )");
  gettoken(ps, es, 0, 0);
  cmd = parseredirs(cmd, ps, es);
  return cmd;
}

struct cmd*
parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  if(peek(ps, es, "("))
    return parseblock(ps, es);

  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;")){
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

// NUL-terminate all the counted strings.
struct cmd*
nulterminate(struct cmd *cmd)
{
  int i;
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    return 0;

  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    nulterminate(lcmd->left);
    nulterminate(lcmd->right);
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    nulterminate(bcmd->cmd);
    break;
  }
  return cmd;
}