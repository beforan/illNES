import React from "react";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import Layout from "@theme/Layout";

function HomepageHeader() {
  const { siteConfig } = useDocusaurusContext();
  return (
    <div>
      <h1>{siteConfig.title}</h1>
      <p>{siteConfig.tagline}</p>
    </div>
  );
}

export default function Home(): JSX.Element {
  const { siteConfig } = useDocusaurusContext();
  return (
    <Layout
      title={`Hello from ${siteConfig.title}`}
      //description="Description will go into a meta tag in <head />"
    >
      <HomepageHeader />
      <main>
        Hello, World!
      </main>
    </Layout>
  );
}
