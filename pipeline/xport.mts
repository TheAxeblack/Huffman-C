import Client, { connect } from "@dagger.io/dagger";
import { resolve } from "path";
import { tarExport } from "./utils.mjs";

/* Version brute */
/* connect(async (client: Client) => {
  const contextDir = client.host().directory(".");

  const binary = client
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
}); */

/* Version avec une fonction */
connect(
  async (client: Client) => {
    await tarExport(client).export(resolve("./bin.tar"));
  },
  { LogOutput: process.stdout }
);
