
[@bs.module "react-copy-to-clipboard"]
  external copyToClipboard : ReasonReact.reactClass = "CopyToClipboard";

let make = (~text: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=copyToClipboard,
    ~props={ "text": text },
    children
  );