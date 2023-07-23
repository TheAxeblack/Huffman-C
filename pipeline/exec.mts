import Client, { connect } from "@dagger.io/dagger";
import { resolve } from "path";

connect(
  async (client: Client) => {
    const contextDir = client.host().directory(".");

    await client
      .container()
      .from("gcc:13.1.0")
      .withWorkdir("/host")
      .withDirectory("/host", contextDir)
      .withExec(["ls", "/host"])
      .withExec(["pwd"])
      .withExec(["make"])
      // .withExec(["/host/bin/my_program", "/host/test.txt"])
      .file("/host/bin/my_program")
      .export(resolve("./bin/my_program"));
    // .stdout();
  },
  { LogOutput: process.stdout }
);
