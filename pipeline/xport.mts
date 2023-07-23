import Client, { connect } from "@dagger.io/dagger";
import { resolve } from "path";

connect(async (client: Client) => {
  const contextDir = client.host().directory(".");

  const binary = await client
    .container()
    .from("gcc:13.1.0")
    .withWorkdir("/host")
    .withDirectory("/host", contextDir)
    .withExec(["make"])
    .file("/host/bin/my_program");

  await client
    .container()
    .from("alpine")
    .withFile("/host/bin/myprogram", binary)
    .withExec(["ls", "/host/bin"])
    .withExec(["tar", "-cvf", "/host/bin.tar", "-C", "/host/bin/", "myprogram"])
    .withExec(["ls", "/host"])
    .file("/host/bin.tar")
    .export(resolve("./bin.tar"));
    //.stdout();
});
