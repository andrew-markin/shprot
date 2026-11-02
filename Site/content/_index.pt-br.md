---
title: "Shprot — proxy SOCKS5 via SSH que se reconecta sozinho"
description: "Aplicativo gratuito para Windows e Linux que mantém um proxy SOCKS5 e HTTP via SSH funcionando: reconecta após mudanças de rede e suspensão, detecta túneis travados, sem linha de comando."
readme: "README.md"

ui:
  skip: "Ir para o conteúdo"
  nav_features: "Recursos"
  nav_compare: "Por que Shprot"
  nav_download: "Baixar"
  nav_faq: "Perguntas"
  language: "Idioma"
  github: "Código-fonte no GitHub"

hero:
  title: "Um proxy SOCKS5 via SSH que continua conectado"
  lead: "O Shprot transforma qualquer servidor que você acessa por SSH em um proxy SOCKS5 e HTTP estável. Ele monitora sua rede, restaura o túnel depois de trocar de Wi-Fi ou sair da suspensão e diz exatamente o que deu errado — sem linha de comando."
  download: "Baixar"
  download_hint: "Versão {version} para Windows e Linux"
  facts:
    - "Gratuito e de código aberto"
    - "Windows 10 / 11 e Linux"
    - "49 idiomas"
  screenshot_alt: "Janela de configurações do Shprot"

features:
  title: "Tudo de que um túnel precisa para continuar funcionando"
  items:
    - id: reconnect
      title: "Aguenta mudanças de rede"
      text: "Trocou de Wi-Fi, conectou um cabo ou acordou o notebook? O Shprot percebe e restaura o túnel sozinho."
    - id: health
      title: "Detecta falhas silenciosas"
      text: "Verificações regulares pelo próprio proxy encontram um túnel que parece conectado mas não passa tráfego, e o reiniciam."
    - id: gentle
      title: "Cuida do seu servidor"
      text: "As novas tentativas ficam cada vez mais espaçadas e, após falhas de login, acontecem no máximo uma vez por minuto, então o fail2ban não vai bloquear seu IP por causa de uma chave errada."
    - id: keys
      title: "Chaves SSH em dois cliques"
      text: "Gere ou importe uma chave e copie a parte pública para o `authorized_keys` direto da janela de configurações."
    - id: trust
      title: "Verificação clara do servidor"
      text: "Na primeira conexão, a impressão digital da chave do servidor aparece em uma caixa de diálogo, para você conferir antes de confiar nele."
    - id: http
      title: "Proxy HTTP também"
      text: "Aplicativos sem suporte a SOCKS5 podem usar um proxy HTTP opcional que passa pelo mesmo túnel."
    - id: tray
      title: "Fica na bandeja do sistema"
      text: "O ícone mostra se o túnel está ativo, as notificações explicam por que não está, e os logs ficam a um clique."
    - id: languages
      title: "Fala o seu idioma"
      text: "A interface está traduzida para 49 idiomas, com layout da direita para a esquerda para árabe, persa, hebraico e urdu."

compare:
  title: "Por que não usar só `ssh -D`?"
  lead: "`ssh -D 1080 user@server` cria um proxy SOCKS5 com um único comando — até a conexão parar de funcionar sem aviso. O Shprot executa o mesmo cliente OpenSSH e cuida de tudo o que o comando sozinho não faz."
  rows:
    - case: "Mudança de rede, troca de Wi-Fi, saída da suspensão"
      ssh: "Reinicie você mesmo"
      shprot: "Reconecta automaticamente"
    - case: "Túnel conectado, mas sem tráfego"
      ssh: "Passa despercebido"
      shprot: "Detectado por verificações pelo proxy"
    - case: "Falhas de autenticação repetidas"
      ssh: "Fica a cargo dos seus scripts"
      shprot: "Tentativas espaçadas para o servidor não bloquear seu IP"
    - case: "Chaves do cliente"
      ssh: "`ssh-keygen` e arquivos em `~/.ssh`"
      shprot: "Gerar ou importar, copiar a chave pública com um clique"
    - case: "Verificação da identidade do servidor"
      ssh: "Pergunta no terminal"
      shprot: "Caixa de diálogo com a impressão digital da chave"
    - case: "Aplicativos sem suporte a SOCKS5"
      ssh: "Sem solução"
      shprot: "Proxy HTTP opcional pelo mesmo túnel"
    - case: "Status do túnel"
      ssh: "Janela do terminal"
      shprot: "Ícone na bandeja e notificações com o motivo da falha"
  note: "Ferramentas como o `autossh` ajudam a reiniciar, mas ainda exigem configuração manual e não têm interface."

uses:
  title: "Para que serve"
  items:
    - id: infra
      title: "Sua própria infraestrutura"
      text: "Acesse painéis internos, ambientes de homologação e ferramentas de administração disponíveis apenas na rede do seu servidor."
    - id: wifi
      title: "Redes não confiáveis"
      text: "Envie seu tráfego pelo seu próprio servidor ao usar Wi-Fi público em cafés, hotéis ou aeroportos."
    - id: laptop
      title: "Trabalho remoto no notebook"
      text: "Tenha um proxy estável em um computador que suspende e troca de rede o dia todo."

how:
  title: "Pronto em três passos"
  steps:
    - title: "Prepare um servidor"
      text: "Qualquer servidor Linux com OpenSSH serve. Nada é instalado nele: um usuário dedicado, que só pode encaminhar portas, é criado com poucos comandos."
    - title: "Conecte o Shprot"
      text: "Informe `user@server`, gere uma chave e adicione a parte pública ao `authorized_keys` do servidor. Confira a impressão digital do servidor uma vez."
    - title: "Aponte os aplicativos para o proxy"
      text: "Use `socks5h://localhost:1080` no navegador ou nas configurações de proxy do sistema — o menu da bandeja copia o endereço para você."
  guide: "Guia passo a passo com proteção do servidor"

tour:
  title: "Olhando de perto"
  items:
    - id: trust
      caption: "Na primeira conexão, você vê a impressão digital da chave do servidor e decide se confia nele."
    - id: tray
      caption: "O menu da bandeja mostra o status do túnel e copia endereços de proxy prontos para usar."

download:
  title: "Baixar o Shprot"
  lead: "Versão {version}. Gratuito, de código aberto, sem contas nem assinaturas."
  windows:
    title: "Windows"
    text: "Windows 10 e 11, 64 bits. Para atualizar, instale a nova versão por cima da antiga."
    button: "Instalador (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ e distribuições compatíveis."
    button: "Pacote (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ e distribuições compatíveis."
    button: "Pacote (.rpm)"
  requirements: "O Shprot usa o cliente OpenSSH (`ssh` e `ssh-keygen`). Ele já vem no Windows 10 / 11 e na maioria dos desktops Linux; se estiver faltando, o Shprot explica como adicioná-lo."
  verify: "Cada versão inclui um arquivo `SHA256SUMS` para verificar se o download não foi alterado."
  all_releases: "Todas as versões e mudanças"

faq:
  title: "Perguntas e respostas"
  items:
    - q: "O Shprot é uma VPN?"
      a: "Não, é um proxy. Só os aplicativos configurados para usá-lo — um navegador, `curl`, `git`, uma IDE — enviam tráfego pelo túnel; todo o resto vai direto. Nenhum adaptador de rede virtual é instalado."
    - q: "O que preciso no servidor?"
      a: "Um servidor OpenSSH comum e uma conta com permissão para encaminhar portas TCP. Nada precisa ser instalado no servidor; qualquer VPS ou servidor doméstico acessível por SSH serve."
    - q: "Posso entrar com senha?"
      a: "Não, o Shprot usa apenas chaves SSH. Chaves são mais seguras que senhas e permitem que o túnel se reconecte sem você. A chave é gerada no aplicativo com um clique."
    - q: "Quais aplicativos podem usar o proxy?"
      a: "Navegadores, `curl`, `git`, gerenciadores de pacotes e qualquer aplicativo com configurações de proxy SOCKS5 ou HTTP ou com suporte às variáveis `ALL_PROXY` / `HTTP_PROXY`. Use `socks5h://` para que os nomes dos sites sejam resolvidos no servidor."
    - q: "Como o Shprot verifica o túnel?"
      a: "De vez em quando, ele abre pelo proxy um site popular escolhido ao acaso. Se isso falhar enquanto a internet está disponível, o túnel é reiniciado."
    - q: "É gratuito?"
      a: "Sim. O Shprot é software livre e de código aberto sob a licença GNU GPL v3, sem contas, assinaturas ou anúncios."
    - q: "Existe versão para macOS?"
      a: "Ainda não. Atualmente o Shprot é compatível com Windows 10 / 11 e Linux."

footer:
  license: "O Shprot é software livre licenciado sob a GNU GPL v3."
  source: "Código-fonte"
  issues: "Relatar um problema"
---
