#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int cod_projeto;
    char nome_projeto[30];
} Projeto;

typedef struct
{
    int cod_tarefa;
    int cod_projeto;
    int data_vencimento;
    int prioridade;
    int status;
    char descricao[50];
} Tarefa;

#define MAX_PROJETOS 10
#define MAX_TAREFAS 50

Projeto projetos[MAX_PROJETOS];
Tarefa tarefas[MAX_TAREFAS];

int num_projetos = 0;
int num_tarefas = 0;

int menu();
void criar();
void adicionar();
void concluir();
void editar();
void excluir();
void listar();
void limparBuffer();
void salvarDados();
void carregarDados();

void imprimirTarefa(int indice_tarefa);
void listar_tudo();
void listar_por_projeto();
void listar_por_prioridade();
void listar_por_data();

int main()
{
    carregarDados();

    int op;
    do
    {
        op = menu();

        if (op == 1)
            criar();
        else if (op == 2)
            adicionar();
        else if (op == 3)
            concluir();
        else if (op == 4)
            editar();
        else if (op == 5)
            excluir();
        else if (op == 6)
            listar();
        else if (op == 7)
        {
            salvarDados();
            printf("\nDados salvos. Saindo do Sistema!\n");
        }

    } while (op != 7);

    return 0;
}

void salvarDados()
{
    FILE *arquivo_projetos, *arquivo_tarefas;

    arquivo_projetos = fopen("projetos.dat", "wb");
    if (arquivo_projetos == NULL)
    {
        printf("\nERRO: Nao foi possivel abrir o arquivo projetos.dat para salvar!\n");
        return;
    }
    fwrite(&num_projetos, sizeof(int), 1, arquivo_projetos);
    fwrite(projetos, sizeof(Projeto), num_projetos, arquivo_projetos);
    fclose(arquivo_projetos);

    arquivo_tarefas = fopen("tarefas.dat", "wb");
    if (arquivo_tarefas == NULL)
    {
        printf("\nERRO: Nao foi possivel abrir o arquivo tarefas.dat para salvar!\n");
        return;
    }
    fwrite(&num_tarefas, sizeof(int), 1, arquivo_tarefas);
    fwrite(tarefas, sizeof(Tarefa), num_tarefas, arquivo_tarefas);
    fclose(arquivo_tarefas);
}

void carregarDados()
{
    FILE *arquivo_projetos, *arquivo_tarefas;

    arquivo_projetos = fopen("projetos.dat", "rb");
    if (arquivo_projetos != NULL)
    {
        fread(&num_projetos, sizeof(int), 1, arquivo_projetos);
        fread(projetos, sizeof(Projeto), num_projetos, arquivo_projetos);
        fclose(arquivo_projetos);
    }

    arquivo_tarefas = fopen("tarefas.dat", "rb");
    if (arquivo_tarefas != NULL)
    {
        fread(&num_tarefas, sizeof(int), 1, arquivo_tarefas);
        fread(tarefas, sizeof(Tarefa), num_tarefas, arquivo_tarefas);
        fclose(arquivo_tarefas);
    }
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int menu()
{
    int m;
    printf("\n\n===== SEJA BEM VINDO! =====");
    printf("\n\n===== MENU DO TO-DO LIST =====");
    printf("\n1 - Criar Projeto");
    printf("\n2 - Adicionar Tarefa");
    printf("\n3 - Concluir Tarefa");
    printf("\n4 - Editar Tarefa");
    printf("\n5 - Excluir Tarefa");
    printf("\n6 - Listar Tarefas (Relatorios)");
    printf("\n7 - Salvar e Sair");
    printf("\n\nDigite a opcao: ");
    scanf("%d", &m);
    limparBuffer();

    while (m < 1 || m > 7)
    {
        printf("\nOpcao Invalida! Digite novamente: ");
        scanf("%d", &m);
        limparBuffer();
    }
    return m;
}

void criar()
{
    printf("\n--- 1. Criar Projeto ---");

    if (num_projetos >= MAX_PROJETOS)
    {
        printf("\nERRO: Limite maximo de projetos atingido (%d)!\n", MAX_PROJETOS);
        return;
    }

    char nome_temp[30];
    printf("\nDigite o nome do projeto: ");
    fgets(nome_temp, 30, stdin);
    nome_temp[strcspn(nome_temp, "\n")] = 0;

    for (int i = 0; i < num_projetos; i++)
    {
        if (strcmp(projetos[i].nome_projeto, nome_temp) == 0)
        {
            printf("\nERRO: Ja existe um projeto com o nome '%s'.\n", nome_temp);
            return;
        }
    }

    strcpy(projetos[num_projetos].nome_projeto, nome_temp);
    projetos[num_projetos].cod_projeto = num_projetos + 1;

    printf("\nProjeto '%s' (Cod: %d) criado com sucesso!", projetos[num_projetos].nome_projeto, projetos[num_projetos].cod_projeto);

    num_projetos++;
}

void adicionar()
{
    printf("\n--- 2. Adicionar Tarefa ---");

    if (num_tarefas >= MAX_TAREFAS)
    {
        printf("\nERRO: Limite maximo de tarefas atingido (%d)!\n", MAX_TAREFAS);
        return;
    }
    if (num_projetos == 0)
    {
        printf("\nERRO: E preciso criar pelo menos um projeto antes de adicionar tarefas!\n");
        return;
    }

    printf("\nProjetos existentes:");
    for (int i = 0; i < num_projetos; i++)
    {
        printf("\n  [%d] %s", projetos[i].cod_projeto, projetos[i].nome_projeto);
    }

    int cod_p_escolhido;
    int projeto_valido = 0;
    printf("\n\nDigite o Codigo do Projeto onde deseja adicionar a tarefa: ");
    scanf("%d", &cod_p_escolhido);
    limparBuffer();

    for (int i = 0; i < num_projetos; i++)
    {
        if (projetos[i].cod_projeto == cod_p_escolhido)
        {
            projeto_valido = 1;
            break;
        }
    }

    if (projeto_valido == 0)
    {
        printf("\nERRO: Codigo de projeto invalido!\n");
        return;
    }

    tarefas[num_tarefas].cod_projeto = cod_p_escolhido;

    printf("Digite a descricao da tarefa (max 50 chars): ");
    fgets(tarefas[num_tarefas].descricao, 50, stdin);
    tarefas[num_tarefas].descricao[strcspn(tarefas[num_tarefas].descricao, "\n")] = 0;

    printf("Digite a data de vencimento (Formato AAMMDD, ex: 251230): ");
    scanf("%d", &tarefas[num_tarefas].data_vencimento);

    printf("Digite a prioridade (1-Alta, 2-Media, 3-Baixa): ");
    scanf("%d", &tarefas[num_tarefas].prioridade);
    limparBuffer();

    tarefas[num_tarefas].status = 1;
    tarefas[num_tarefas].cod_tarefa = num_tarefas + 1;

    printf("\nTarefa '%s' (Cod: %d) adicionada ao projeto %d!", tarefas[num_tarefas].descricao, tarefas[num_tarefas].cod_tarefa, tarefas[num_tarefas].cod_projeto);

    num_tarefas++;
}

void listar()
{
    int op_listar;
    do
    {
        printf("\n\n--- 6. Menu de Listagem  ---");
        printf("\n1 - Listar tudo (separado por projeto)");
        printf("\n2 - Listar tarefas de um projeto especifico");
        printf("\n3 - Listar tarefas por prioridade");
        printf("\n4 - Listar tarefas por data de vencimento");
        printf("\n5 - Voltar ao Menu Principal");
        printf("\n\nDigite a opcao de listagem: ");
        scanf("%d", &op_listar);
        limparBuffer();

        if (op_listar == 1)
        {
            listar_tudo();
        }
        else if (op_listar == 2)
        {
            listar_por_projeto();
        }
        else if (op_listar == 3)
        {
            listar_por_prioridade();
        }
        else if (op_listar == 4)
        {
            listar_por_data();
        }
        else if (op_listar == 5)
        {
            printf("\nVoltando ao Menu Principal...");
        }
        else
        {
            printf("\nOpcao de listagem invalida!\n");
        }

    } while (op_listar != 5);
}

void imprimirTarefa(int j)
{
    char status_texto[20];
    char nome_proj[30] = "PROJETO NAO ENCONTRADO";

    if (tarefas[j].status == 1)
        strcpy(status_texto, "PENDENTE");
    else
        strcpy(status_texto, "CONCLUIDA");

    for (int p = 0; p < num_projetos; p++)
    {
        if (projetos[p].cod_projeto == tarefas[j].cod_projeto)
        {
            strcpy(nome_proj, projetos[p].nome_projeto);
            break;
        }
    }

    printf("\n\n  Tarefa %d: %s", tarefas[j].cod_tarefa, tarefas[j].descricao);
    printf("\n    Projeto: %s (Cod: %d)", nome_proj, tarefas[j].cod_projeto);
    printf("\n    Status: %s | Prioridade: %d | Vencimento: %d", status_texto, tarefas[j].prioridade, tarefas[j].data_vencimento);
}

void listar_tudo()
{
    printf("\n--- Listando Todos os Projetos e Tarefas ---");

    if (num_projetos == 0)
    {
        printf("\nNenhum projeto cadastrado.\n");
        return;
    }

    for (int i = 0; i < num_projetos; i++)
    {
        printf("\n\n=============================================");
        printf("\nPROJETO %d: %s", projetos[i].cod_projeto, projetos[i].nome_projeto);
        printf("\n=============================================");

        int tarefas_encontradas_neste_projeto = 0;

        for (int j = 0; j < num_tarefas; j++)
        {
            if (tarefas[j].cod_projeto == projetos[i].cod_projeto)
            {
                tarefas_encontradas_neste_projeto = 1;
                imprimirTarefa(j);
            }
        }

        if (tarefas_encontradas_neste_projeto == 0)
        {
            printf("\n  (Nenhuma tarefa neste projeto)");
        }
    }
}

void listar_por_projeto()
{
    printf("\n--- Listar por Projeto Especifico ---");
    if (num_projetos == 0)
    {
        printf("\nNenhum projeto cadastrado.\n");
        return;
    }

    printf("\nProjetos existentes:");
    for (int i = 0; i < num_projetos; i++)
    {
        printf("\n  [%d] %s", projetos[i].cod_projeto, projetos[i].nome_projeto);
    }

    int cod_p_escolhido;
    printf("\n\nDigite o Codigo do Projeto que deseja listar: ");
    scanf("%d", &cod_p_escolhido);
    limparBuffer();

    int tarefas_encontradas = 0;
    printf("\n--- Tarefas do Projeto %d ---", cod_p_escolhido);
    for (int j = 0; j < num_tarefas; j++)
    {
        if (tarefas[j].cod_projeto == cod_p_escolhido)
        {
            imprimirTarefa(j);
            tarefas_encontradas++;
        }
    }

    if (tarefas_encontradas == 0)
    {
        printf("\nNenhuma tarefa encontrada para este projeto.\n");
    }
}

void listar_por_prioridade()
{
    printf("\n--- Listar por Prioridade ---");
    int prioridade_desejada;
    printf("\nDigite a prioridade que deseja filtrar (1-Alta, 2-Media, 3-Baixa): ");
    scanf("%d", &prioridade_desejada);
    limparBuffer();

    int tarefas_encontradas = 0;
    printf("\n--- Tarefas com Prioridade %d ---", prioridade_desejada);
    for (int j = 0; j < num_tarefas; j++)
    {
        if (tarefas[j].prioridade == prioridade_desejada)
        {
            imprimirTarefa(j);
            tarefas_encontradas++;
        }
    }

    if (tarefas_encontradas == 0)
    {
        printf("\nNenhuma tarefa encontrada com esta prioridade.\n");
    }
}

void listar_por_data()
{
    printf("\n--- Listar por Data de Vencimento ---");
    int data_desejada;
    printf("\nDigite a data que deseja filtrar (Formato AAMMDD): ");
    scanf("%d", &data_desejada);
    limparBuffer();

    int tarefas_encontradas = 0;
    printf("\n--- Tarefas com Vencimento em %d ---", data_desejada);
    for (int j = 0; j < num_tarefas; j++)
    {
        if (tarefas[j].data_vencimento == data_desejada)
        {
            imprimirTarefa(j);
            tarefas_encontradas++;
        }
    }

    if (tarefas_encontradas == 0)
    {
        printf("\nNenhuma tarefa encontrada com esta data de vencimento.\n");
    }
}

void concluir()
{
    printf("\n--- 3. Concluir Tarefa ---");

    if (num_tarefas == 0)
    {
        printf("\nNenhuma tarefa cadastrada para concluir.\n");
        return;
    }

    printf("\nTarefas Pendentes:");
    int pendentes = 0;
    for (int i = 0; i < num_tarefas; i++)
    {
        if (tarefas[i].status == 1)
        {
            printf("\n  [Cod: %d] %s", tarefas[i].cod_tarefa, tarefas[i].descricao);
            pendentes++;
        }
    }
    if (pendentes == 0)
    {
        printf("\nNenhuma tarefa pendente encontrada.\n");
        return;
    }

    int cod_t_escolhido;
    printf("\n\nDigite o Codigo da Tarefa que deseja concluir: ");
    scanf("%d", &cod_t_escolhido);
    limparBuffer();

    int encontrada = 0;
    for (int i = 0; i < num_tarefas; i++)
    {
        if (tarefas[i].cod_tarefa == cod_t_escolhido)
        {
            tarefas[i].status = 2;
            encontrada = 1;
            printf("\nTarefa %d ('%s') marcada como CONCLUIDA!", tarefas[i].cod_tarefa, tarefas[i].descricao);
            break;
        }
    }

    if (encontrada == 0)
    {
        printf("\nERRO: Tarefa com Codigo %d nao encontrada.\n", cod_t_escolhido);
    }
}

void editar()
{
    printf("\n--- 4. Editar Tarefa ---");

    if (num_tarefas == 0)
    {
        printf("\nNenhuma tarefa cadastrada para editar.\n");
        return;
    }

    listar_tudo();

    int cod_t_escolhido;
    printf("\n\nDigite o Codigo da Tarefa que deseja editar: ");
    scanf("%d", &cod_t_escolhido);
    limparBuffer();

    int encontrada = 0;
    for (int i = 0; i < num_tarefas; i++)
    {
        if (tarefas[i].cod_tarefa == cod_t_escolhido)
        {
            encontrada = 1;

            printf("\nEditando Tarefa: %s", tarefas[i].descricao);
            printf("\n(Pressione ENTER para manter o valor atual)\n");

            char nova_descricao[50];
            char temp_input[20];

            printf("  Nova Descricao (Atual: %s): ", tarefas[i].descricao);
            fgets(nova_descricao, 50, stdin);
            if (nova_descricao[0] != '\n')
            {
                nova_descricao[strcspn(nova_descricao, "\n")] = 0;
                strcpy(tarefas[i].descricao, nova_descricao);
            }

            printf("  Nova Data Vencimento (Atual: %d): ", tarefas[i].data_vencimento);
            fgets(temp_input, 20, stdin);
            if (temp_input[0] != '\n')
            {
                sscanf(temp_input, "%d", &tarefas[i].data_vencimento);
            }

            printf("  Nova Prioridade (Atual: %d): ", tarefas[i].prioridade);
            fgets(temp_input, 20, stdin);
            if (temp_input[0] != '\n')
            {
                sscanf(temp_input, "%d", &tarefas[i].prioridade);
            }

            printf("\nTarefa %d atualizada com sucesso!", tarefas[i].cod_tarefa);
            break;
        }
    }

    if (encontrada == 0)
    {
        printf("\nERRO: Tarefa com Codigo %d nao encontrada.\n", cod_t_escolhido);
    }
}

void excluir()
{
    printf("\n--- 5. Excluir Tarefa ---");

    if (num_tarefas == 0)
    {
        printf("\nNenhuma tarefa cadastrada para excluir.\n");
        return;
    }

    listar_tudo();

    int cod_t_escolhido;
    printf("\n\nDigite o Codigo da Tarefa que deseja EXCLUIR (PERMANENTEMENTE): ");
    scanf("%d", &cod_t_escolhido);
    limparBuffer();

    int indice_encontrado = -1;

    for (int i = 0; i < num_tarefas; i++)
    {
        if (tarefas[i].cod_tarefa == cod_t_escolhido)
        {
            indice_encontrado = i;
            printf("\nTarefa '%s' encontrada para exclusao.", tarefas[i].descricao);
            break;
        }
    }

    if (indice_encontrado != -1)
    {
        for (int i = indice_encontrado; i < num_tarefas - 1; i++)
        {
            tarefas[i] = tarefas[i + 1];
        }

        num_tarefas--;

        printf("\nTarefa excluida com sucesso!");
    }
    else
    {
        printf("\nERRO: Tarefa com Codigo %d nao encontrada.\n", cod_t_escolhido);
    }
}
