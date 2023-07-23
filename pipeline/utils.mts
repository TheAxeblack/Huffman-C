import Client from "@dagger.io/dagger";

export function build(client: Client) {
  const contextDir = client.host().directory(".");
  const binary = client
    .container()
    .from("gcc:13.1.0")
    .withWorkdir("/host")
    .withDirectory("/host", contextDir)
    .withExec(["make"])
    .file("/host/bin/my_program");
  return binary;
}

export function tarExport(client: Client) {
  const binary = build(client);

  const tarArchive = client
    .container()
    .from("alpine")
    .withFile("/host/bin/myprogram", binary)
    .withExec(["tar", "-cvf", "/host/bin.tar", "-C", "/host/bin/", "myprogram"])
    .file("/host/bin.tar");
  return tarArchive;
}
