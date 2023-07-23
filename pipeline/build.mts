import Client, { connect } from "@dagger.io/dagger";
import { resolve } from "path";

import { build } from "./utils.mjs";

/* Version brute */
/* connect(
  async (client: Client) => {
    const contextDir = client.host().directory(".");

    await client
      .container()
      .from("gcc:13.1.0")
      .withWorkdir("/host")
      .withDirectory("/host", contextDir)
      .withExec(["make"])
      .file("/host/bin/my_program")
      .export(resolve("./bin/my_program"));
    // .stdout();
  },
  { LogOutput: process.stdout }
); */

/* Version avec une fonction */
connect(
  async (client: Client) => {
    await build(client).export(resolve("./bin/my_program"));
  },
  { LogOutput: process.stdout }
);
